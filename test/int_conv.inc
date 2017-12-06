ICT(u16_to_u8_00, u16_to_u8(0), 0, 0);
ICT(u16_to_u8_FF, u16_to_u8(0xFF), 0, 0xFF);
ICT(u16_to_u8_100, u16_to_u8(0x100), 1, 0);
ICT(u8_from_u16_00, u8_from_u16(0), 0, 0);
ICT(u8_from_u16_FF, u8_from_u16(0xFF), 0, 0xFF);
ICT(u8_from_u16_100, u8_from_u16(0x100), 1, 0);
ICT(cast_u8_from_u16_123, cast_u8_from_u16(0x123), 0, 0x23);
ICT(cast_u16_to_u8_234, cast_u16_to_u8(0x234), 0, 0x34);

ICT(u32_to_u8_00, u32_to_u8(0), 0, 0);
ICT(u32_to_u8_FF, u32_to_u8(0xFF), 0, 0xFF);
ICT(u32_to_u8_100, u32_to_u8(0x100), 1, 0);
ICT(u32_to_u8_2G, u32_to_u8(0x80000000), 1, 0);
ICT(u8_from_u32_00, u8_from_u32(0), 0, 0);
ICT(u8_from_u32_FF, u8_from_u32(0xFF), 0, 0xFF);
ICT(u8_from_u32_100, u8_from_u32(0x100), 1, 0);
ICT(cast_u8_from_u32_12345678, cast_u8_from_u32(0x12345678), 0, 0x78);
ICT(cast_u32_to_u8_23456, cast_u32_to_u8(0x23456), 0, 0x56);

ICT(u64_to_u8_00, u64_to_u8(0), 0, 0);
ICT(u64_to_u8_FF, u64_to_u8(0xFF), 0, 0xFF);
ICT(u64_to_u8_100, u64_to_u8(0x100), 1, 0);
ICT(u64_to_u8_2G, u64_to_u8(0x80000000), 1, 0);
ICT(u8_from_u64_00, u8_from_u64(0), 0, 0);
ICT(u8_from_u64_FF, u8_from_u64(0xFF), 0, 0xFF);
ICT(u8_from_u64_100, u8_from_u64(0x100), 1, 0);
ICT(cast_u8_from_u64_12345678, cast_u8_from_u64(UINT64_C(0xCC12345678)), 0, 0x78);
ICT(cast_u64_to_u8_23456, cast_u64_to_u8(0x23456), 0, 0x56);

ICT(u8_to_s8_00, u8_to_s8(0), 0, 0);
ICT(u8_to_s8_11, u8_to_s8(0x11), 0, 0x11);
ICT(u8_to_s8_80, u8_to_s8(0x80), 1, 0);
ICT(u8_to_s8_FF, u8_to_s8(0xFF), 1, 0);

ICT(s8_from_u8_00, s8_from_u8(0), 0, 0);
ICT(s8_from_u8_11, s8_from_u8(0x11), 0, 0x11);
ICT(s8_from_u8_80, s8_from_u8(0x80), 1, 0);
ICT(s8_from_u8_FF, s8_from_u8(0xFF), 1, 0);

ICT(cast_u8_to_s8_80, cast_u8_to_s8(0x80), 0, -0x80);
ICT(cast_s8_from_u8_80, cast_s8_from_u8(0x80), 0, -0x80);

ICT(u16_to_s8_00, u16_to_s8(0), 0, 0);
ICT(u16_to_s8_11, u16_to_s8(0x11), 0, 0x11);
ICT(u16_to_s8_80, u16_to_s8(0x80), 1, 0);
ICT(u16_to_s8_100, u16_to_s8(0x100), 1, 0);
ICT(u16_to_s8_FFFF, u16_to_s8(0xFFFF), 1, 0);

ICT(s8_from_u16_00, s8_from_u16(0), 0, 0);
ICT(s8_from_u16_11, s8_from_u16(0x11), 0, 0x11);
ICT(s8_from_u16_80, s8_from_u16(0x80), 1, 0);
ICT(s8_from_u16_FFFF, s8_from_u16(0xFFFF), 1, 0);

ICT(cast_u16_to_s8_80, cast_u16_to_s8(0x80), 0, -0x80);
ICT(cast_u16_to_s8_C080, cast_u16_to_s8(0xC080), 0, -0x80);
ICT(cast_s8_from_u16_80, cast_s8_from_u16(0x80), 0, -0x80);

ICT(uptr_to_u8_FF, uptr_to_u8(0xFF), 0, 0xFF);
ICT(uptr_to_u16_10000, uptr_to_u16(0x10000), 1, 0);


/* vim: set ft=c: */
