ICT(u16_to_u8_00, u16_to_u8(0), 0, 0);
ICT(u16_to_u8_FF, u16_to_u8(0xFF), 0, 0xFF);
ICT(u16_to_u8_100, u16_to_u8(0x100), 1, 0);

ICT(u8_from_u16_00, u8_from_u16(0), 0, 0);
ICT(u8_from_u16_FF, u8_from_u16(0xFF), 0, 0xFF);
ICT(u8_from_u16_100, u8_from_u16(0x100), 1, 0);

ICT(trunc_u8_from_u16_123, trunc_u8_from_u16(0x123), 0, 0x23);
ICT(trunc_u16_to_u8_234, trunc_u16_to_u8(0x234), 0, 0x34);
