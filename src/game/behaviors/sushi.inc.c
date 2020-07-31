// sushi.c.inc

void bhv_sushi_shark_collision_loop(void)
{
}

void bhv_sushi_shark_loop(void)
{
	f32 sp1C	 = find_water_level(o->oPosX, o->oPosZ);
	o->oPosX	 = o->oHomeX + sins(o->oSushiSharkUnkF4) * 1700.0f;
	o->oPosZ	 = o->oHomeZ + coss(o->oSushiSharkUnkF4) * 1700.0f;
	o->oPosY	 = sp1C + o->oHomeY + sins(o->oSushiSharkUnkF4) * 200.0f;
	o->oMoveAngleYaw = o->oSushiSharkUnkF4 + 0x4000;
	o->oSushiSharkUnkF4 += 0x80 / FRAME_RATE_SCALER_INV;

	if(gMarioObject->oPosY - sp1C > -500.0f)
		if(o->oPosY - sp1C > -200.0f)
			s_makeobj_nowpos_scale(o, MODEL_WATER_WAVES, sm64::bhv::bhvWaterType(), 4.0f);
	if((o->oTimer & 0xF) == 0)
		objsound(SOUND_OBJ_SUSHI_SHARK_WATER_SOUND);
	o->oInteractStatus = 0;
}
