#ifndef PERSISTENT_LOG_H
#define PERSISTENT_LOG_H

#include <Arduino.h>
#include <EEPROM.h>

static const uint16_t LOG_MAGIC_ADDR = 0;
static const uint16_t LOG_HEAD_ADDR = 2;   // next write position in data region
static const uint16_t LOG_COUNT_ADDR = 4;  // number of valid bytes in data region
static const uint16_t LOG_DATA_START = 6;
static const uint16_t LOG_DATA_SIZE = 1024; // Mega2560 EEPROM is 4096 bytes; reserve 1KB for logs
static const uint16_t LOG_MAGIC = 0xB10F;

static inline uint16_t logDataEnd() { return (uint16_t)(LOG_DATA_START + LOG_DATA_SIZE); }

static inline uint16_t readU16(uint16_t addr) {
  uint16_t v = EEPROM.read(addr);
  v |= (uint16_t)EEPROM.read(addr + 1) << 8;
  return v;
}

static inline void writeU16(uint16_t addr, uint16_t value) {
  EEPROM.update(addr, (uint8_t)(value & 0xFF));
  EEPROM.update(addr + 1, (uint8_t)((value >> 8) & 0xFF));
}

static inline void logEnsureInit() {
  if (readU16(LOG_MAGIC_ADDR) != LOG_MAGIC) {
    writeU16(LOG_MAGIC_ADDR, LOG_MAGIC);
    writeU16(LOG_HEAD_ADDR, LOG_DATA_START);
    writeU16(LOG_COUNT_ADDR, 0);
  }
}

static inline void logClear() {
  writeU16(LOG_HEAD_ADDR, LOG_DATA_START);
  writeU16(LOG_COUNT_ADDR, 0);
}

static inline uint16_t logHead() { return readU16(LOG_HEAD_ADDR); }
static inline uint16_t logCount() { return readU16(LOG_COUNT_ADDR); }

static inline uint16_t logAdvance(uint16_t p) {
  p++;
  if (p >= logDataEnd()) p = LOG_DATA_START;
  return p;
}

static inline void logPushByte(uint8_t b) {
  uint16_t head = logHead();
  uint16_t count = logCount();

  EEPROM.update(head, b);
  head = logAdvance(head);

  if (count < LOG_DATA_SIZE) count++;
  writeU16(LOG_HEAD_ADDR, head);
  writeU16(LOG_COUNT_ADDR, count);
}

static inline void logAppend(const char *msg) {
  if (!msg) return;
  while (*msg) {
    logPushByte((uint8_t)*msg++);
  }
  logPushByte((uint8_t)'\n');
}

static inline void logAppendKV(const char *prefix, int value) {
  char buf[40];
  snprintf(buf, sizeof(buf), "%s%d", prefix, value);
  logAppend(buf);
}

static inline void logDumpToSerial(Stream &out) {
  uint16_t head = logHead();
  uint16_t count = logCount();
  if (count == 0) {
    out.println(F("[EEPROM LOG] <empty>"));
    return;
  }

  uint16_t start = head;
  if (count < LOG_DATA_SIZE) {
    start = (uint16_t)(LOG_DATA_START + (head - LOG_DATA_START + LOG_DATA_SIZE - count) % LOG_DATA_SIZE);
  }

  out.println(F("[EEPROM LOG] ===== BEGIN ====="));
  uint16_t p = start;
  for (uint16_t i = 0; i < count; i++) {
    out.write(EEPROM.read(p));
    p = logAdvance(p);
  }
  out.println();
  out.println(F("[EEPROM LOG] ===== END ====="));
}

#endif // PERSISTENT_LOG_H
