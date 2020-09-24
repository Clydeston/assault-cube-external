#pragma once

#define local_entity_object 0x10F4F4
#define entity_health 0xF8
#define entity_name  0x225
#define entity_armour 0xFC
#define entity_team 0x32C
#define entity_jump 0x69 
#define entity_speed 0x80  
#define entity_x 0x34
#define entity_y 0x38
#define entity_z 0x3C
#define entity_head_x 0x4
#define entity_head_y 0x8
#define entity_head_z 0xC
#define view_matrix 0x501AE8

#define ammo_address 0x637e9
#define recoil_address 0x63786
#define grenade_ammo 0x158

#define entity_list 0x10F4F8
#define entity_byte_size 0x4
#define entity_count 0x50F500

// ff 06 = inc [esi]
#define ammo_increase_instruction "\xFF\x06"
// ff 0E = dec [esi]
#define ammo_decrease_instruction "\xFF\x0E"
#define original_recoil_instruction "\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2"