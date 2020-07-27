// grill_door.c.inc

extern const Collision bob_seg7_collision_gate[];
extern const Collision hmc_seg7_collision_0702B65C[];
struct Struct8032FCE8 D_8032FCE8[] = {{320, MODEL_BOB_BARS_GRILLS, bob_seg7_collision_gate}, {410, MODEL_HMC_RED_GRILLS, hmc_seg7_collision_0702B65C}};

void bhv_openable_cage_door_loop(void)
{
	if(gCurrentObject->oAction == 0)
	{
		if(gCurrentObject->parentObj->oOpenableGrillUnk88 != 0)
			gCurrentObject->oAction++;
	}
	else if(gCurrentObject->oAction == 1)
	{
		if(gCurrentObject->oTimer < 64 * FRAME_RATE_SCALER_INV)
			gCurrentObject->oMoveAngleYaw -= gCurrentObject->oBehParams2ndByte * 0x100 / FRAME_RATE_SCALER_INV;
		else
			gCurrentObject->oAction++;
	}
}

void bhv_openable_grill_loop(void)
{
	struct Object* sp3C;
	s32 sp38;
	switch(o->oAction)
	{
		case 0:
			sp38 = o->oBehParams2ndByte;
			sp3C = s_makeobj_chain(-1, D_8032FCE8[sp38].unk0, 0, 0, o, D_8032FCE8[sp38].unk1, sm64::bhv::bhvOpenableCageDoor());
			sp3C->oMoveAngleYaw += 0x8000;
			set_object_collision_data(sp3C, D_8032FCE8[sp38].unk2);
			sp3C = s_makeobj_chain(1, -D_8032FCE8[sp38].unk0, 0, 0, o, D_8032FCE8[sp38].unk1, sm64::bhv::bhvOpenableCageDoor());
			set_object_collision_data(sp3C, D_8032FCE8[sp38].unk2);
			o->oAction++;
			break;
		case 1:
			if((o->oOpenableGrillUnkF4 = s_find_obj(sm64::bhv::bhvFloorSwitchGrills())) != NULL)
				o->oAction++;
			break;
		case 2:
			sp3C = o->oOpenableGrillUnkF4;
			if(sp3C->oAction == 2)
			{
				o->oOpenableGrillUnk88 = 2;
				objsound(SOUND_GENERAL_CAGE_OPEN);
				o->oAction++;
				if(o->oBehParams2ndByte != 0)
					Na_NazoClearBgm();
			}
			break;
		case 3:
			break;
	}
}
