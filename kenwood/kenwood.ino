/*
 * 2005 VOLVO XC70 steering wheel buttons interface for Kenwood head unit
 *
 * (c) 2014 Vitaly Mayatskikh vitaly@gravicappa.info
 *
 *
 * Steering wheel module CAN ID is 0x0131726c
 * Message is 8 bytes long: 0x00 0x0c 0x28 0x52 0x80 0x00 0x00 0x3f
 *                                                                ^
 *                                                these 4 bits are audio buttons
 * bit 0 - previous track
 * bit 1 - next track
 * bit 2 - volume down
 * bit 3 - volume up
 *
 * 40 messages per second
 *
 * Kenwood codes were taken from here: http://custombaggerforum.com/forum/showthread.php?551-Road-Glide-Hacks&p=7085&viewfull=1#post7085
 * Thanks, Len Shelton!
 *
 * CCM (?) CAN ID 0x02803008
 * Sends ambient light level
 * Message is 8 bytes long: 0x8D 0x41 0x90 0xF9 0xFF 0x00 0x40 0x00
 *                                           ^^
 *                                           |Instrument cluster backlight 0..F
 *                                           Ambient light (F - day, !F - not day)
 *
 * CEM (?) CAN ID 0x03200408
 * Gearbox selector position
 * Message is 8 bytes long: 0x11 0xFE 0x13 0x00 0x24 0x00 0x10 0x00
 *                                                          ^
 *                                                          2 bits
 * 1 - P
 * 2 - R
 * 3 - N
 * 4 - D
 *
 * 10 messages per second
 *
 */

#include <SPI.h>
#include <mcp_can.h>
#include <mcp_can_dfs.h>

#define SWM_CAN_ID 0x0131726c
#define CCM_CAN_ID 0x02803008
#define CEM_CAN_ID 0x03200408

#define CAN_MASK (SWM_CAN_ID | CCM_CAN_ID | CEM_CAN_ID)

/* direct button mappings */
#define SWM_TRK_PREV    (1ULL << (8 * 7 + 0))
#define SWM_TRK_NEXT    (1ULL << (8 * 7 + 1))
#define SWM_VOL_DOWN    (1ULL << (8 * 7 + 2))
#define SWM_VOL_UP      (1ULL << (8 * 7 + 3))

/* combo mappings for extra functions */
#define COMBO_DISC_PREV (SWM_TRK_PREV | SWM_VOL_DOWN)
#define COMBO_DISC_NEXT (SWM_TRK_NEXT | SWM_VOL_UP)
//#define COMBO_ILLUMI    (SWM_TRK_PREV | SWM_VOL_UP)
#define COMBO_REBOOT    (SWM_TRK_PREV | SWM_VOL_UP)

#define WD_OUTPUT     8  // MAX823/MAX824 pin 4 (WDI)
#define SWC_OUTPUT    7  // Steering remote control wire
#define CAN_RESET     6  // MCP2515 pin 17 (-Reset)
#define ILLUMI_OUTPUT 5  // Dimmer control wire (through 5->12v NPN transistor amplifier)
#define PARK_OUTPUT   4  // Gearbox is in parking position (through 5->12v NPN transistor amplifier)
#define CAMERA_OUTPUT 3  // Gearbox is in reverse position (through 5->12v NPN transistor amplifier)

#define NR_ACTIONS 7

MCP_CAN CAN(10);

unsigned char cmd_common[] = { 0,1,0,0,0,1,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,0,1,0,0 };

struct action {
  unsigned long long mask;
  void (*on_press)(struct action *);
  void (*on_release)(struct action *);
  char msg[16];
  char opaque[32];
};

struct swc_action {
  boolean kenwood_cmd[24];
};

struct toggle_action {
  unsigned char pin;
  boolean last_state;
  boolean pin_state;
};

struct funcall_action {
  unsigned char idx;
};

void swc_action_on_press(struct action *a)
{
  struct swc_action *s = (struct swc_action *)a->opaque;
  Serial.println(a->msg);
  send(s->kenwood_cmd);
}

void toggle_action_on_press(struct action *a)
{
  struct toggle_action *t = (struct toggle_action *)a->opaque;
  if (t->last_state == false) {
    t->last_state = true;
    t->pin_state = !t->pin_state;
    Serial.print(a->msg); Serial.println(t->pin_state);
    digitalWrite(t->pin, t->pin_state);
  }
}

void delay_on_release(struct action *a)
{
  unsigned char len;
  unsigned char buf[8];

  Serial.println("delay");
  delay(1000); // give human some time to release combo button
  while (CAN.checkReceive() == CAN_MSGAVAIL) {
    CAN.readMsgBuf(&len, buf);
  }
}

void toggle_action_on_release(struct action *a)
{
  struct toggle_action *t = (struct toggle_action *)a->opaque;
  t->last_state = false;
  delay_on_release(a);
}

void funcall_action_on_press(struct action *a)
{
  struct funcall_action *f = (struct funcall_action *)a->opaque;
  switch(f->idx) {
    case COMBO_REBOOT:
      delay(3000); // hw watchdog kills us
      break;
    default:
      break;
  }
}

struct action actions[NR_ACTIONS] =
  {
//    { COMBO_ILLUMI,    toggle_action_on_press, toggle_action_on_release, "ILLUMI",   { ILLUMI_OUTPUT, false, true } },
    { COMBO_REBOOT,    funcall_action_on_press,NULL,                     "REBOOT",   { COMBO_REBOOT } },
    { COMBO_DISC_PREV, swc_action_on_press,    delay_on_release,         "DSC_PREV", { 0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0 } },
    { COMBO_DISC_NEXT, swc_action_on_press,    delay_on_release,         "DSC_NEXT", { 1,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0 } },
    { SWM_TRK_PREV,    swc_action_on_press,    NULL,                     "TRK_PREV", { 0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,1,0,1,0,1,0 } },
    { SWM_TRK_NEXT,    swc_action_on_press,    NULL,                     "TRK_NEXT", { 1,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0 } },
    { SWM_VOL_DOWN,    swc_action_on_press,    NULL,                     "VOL_DOWN", { 1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,1,0,1,0 } },
    { SWM_VOL_UP,      swc_action_on_press,    NULL,                     "VOL_UP",   { 0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0 } },
};

struct action *last_action = NULL;

void send(boolean *command)
{
  pinMode(SWC_OUTPUT, INPUT);
  pinMode(SWC_OUTPUT, OUTPUT);
  digitalWrite(SWC_OUTPUT, LOW);
  delay(10);
  digitalWrite(SWC_OUTPUT, HIGH);
  delayMicroseconds(4500);

  for (int i = 0; i < 25; i++)
    send_bit(cmd_common[i]);
  for (int i = 0; i < 24; i++)
    send_bit(command[i]);
}

void send_bit(boolean bit)
{
  digitalWrite(SWC_OUTPUT, bit);
  delayMicroseconds(1000);
  digitalWrite(SWC_OUTPUT, HIGH);
  delayMicroseconds(200);
}

boolean can_ok = false;

void can_reset()
{
  pinMode(CAN_RESET, OUTPUT);
  digitalWrite(CAN_RESET, LOW);
  delay(100);
  digitalWrite(CAN_RESET, HIGH);
  delay(100);
}

void touch_watchdog()
{
  static bool state = true;
  pinMode(WD_OUTPUT, OUTPUT);
  digitalWrite(WD_OUTPUT, state);
  state = !state;
}

void setup_can()
{
  Serial.println("CAN BUS Shield initialize...");
  can_reset();

  for (int i = 0; i < 4; i++) {
    touch_watchdog();
    if (CAN.begin(CAN_125KBPS) == CAN_OK) {
      can_ok = true;
      break;
    }
    Serial.print(".");
    delay(250);
  }

  if (can_ok) {
    int err = 0;
    Serial.println("... inited!");
    Serial.println("CAN BUS Shield setting filter");
    if (CAN.init_Mask(0, 1, CAN_MASK) != MCP2515_OK) {
      Serial.println("init mask 0 failed!");
      err++;
    }
    if (CAN.init_Mask(1, 1, CAN_MASK) != MCP2515_OK) {
      Serial.println("init mask 1 failed!");
      err++;
    }
    if (CAN.init_Filt(0, 1, SWM_CAN_ID) != MCP2515_OK) {
      Serial.println("init filter 0 failed!");
      err++;
    }
    if (CAN.init_Filt(1, 1, CCM_CAN_ID) != MCP2515_OK) {
      Serial.println("init filter 1 failed!");
      err++;
    }
    if (CAN.init_Filt(2, 1, CEM_CAN_ID) != MCP2515_OK) {
      Serial.println("init filter 2 failed!");
      err++;
    }
    if (err)
      can_ok = false;
  }

  if (!can_ok) {
    Serial.println("... failed!");
    delay(3000); // hw wd kills us
  }
}

void setup()
{
  touch_watchdog();
  Serial.begin(9600);
  Serial.println("start");

  pinMode(SWC_OUTPUT, OUTPUT);
  digitalWrite(SWC_OUTPUT, HIGH);
  pinMode(ILLUMI_OUTPUT, OUTPUT);
  digitalWrite(ILLUMI_OUTPUT, HIGH);
  pinMode(PARK_OUTPUT, OUTPUT);
  digitalWrite(PARK_OUTPUT, HIGH);
  pinMode(CAMERA_OUTPUT, OUTPUT);
  digitalWrite(CAMERA_OUTPUT, HIGH);

  Serial.println("Kenwood SWC inited");

  setup_can();
  touch_watchdog();
  Serial.println("All set!");
}

void loop()
{
  if (can_ok)
    check_canbus();
}

void check_canbus()
{
  unsigned char len;
  unsigned char buf[8];

  if(CAN.checkReceive() == CAN_MSGAVAIL) {
    memset(buf, 0, sizeof(buf));
    CAN.readMsgBuf(&len, buf);
    switch (CAN.getCanId()) {
      case SWM_CAN_ID:
	touch_watchdog();
	do_actions(buf);
	break;
      case CCM_CAN_ID:
	pinMode(ILLUMI_OUTPUT, OUTPUT);
	digitalWrite(ILLUMI_OUTPUT, ((buf[3] & 0xf0) == 0xf0) ? HIGH : LOW);
	break;
      case CEM_CAN_ID:
	pinMode(PARK_OUTPUT, OUTPUT);
	digitalWrite(PARK_OUTPUT, ((buf[6] & 0x30) == 0x10) ? LOW : HIGH);
	pinMode(CAMERA_OUTPUT, OUTPUT);
	digitalWrite(CAMERA_OUTPUT, ((buf[6] & 0x30) == 0x20) ? LOW : HIGH);
	break;
      default:
	break;
    }
  }
}

void do_actions(unsigned char *buf)
{
  int i;

  for (i = 0; i < NR_ACTIONS; i++) {
    if (do_action(i, buf))
      break;
  }
  if (i == NR_ACTIONS) {  // no buttons pressed
    if (last_action) {
      if (last_action->on_release)
	last_action->on_release(last_action);
      last_action = NULL;
    }
  }
}

boolean do_action(unsigned char idx, unsigned char *buf)
{
  boolean ret = false;
  struct action *a = &actions[idx];
  unsigned char *mask = (unsigned char *)&a->mask;
  int sum = 0;

  for (int i = 0; i < 8; i++)
    sum += buf[i] & mask[i];

  if (sum == 0) {
    if (last_action && last_action != a && last_action->on_release)
      last_action->on_release(last_action);
    if (a->on_press)
      a->on_press(a);
    last_action = a;
    ret = true;
  }
  return ret;
}
