// flame_mario.c.inc

void bhv_black_smoke_upward_loop(void)
{
	spawn_object_with_scale(o, MODEL_BURN_SMOKE, sm64::bhv::bhvBlackSmokeBowser(), o->header.gfx.scale[0]);
}

void bhv_black_smoke_bowser_loop(void)
{
	if(o->oTimer == 0)
	{
		o->oForwardVel		  = Randomf() * 2 + 0.5;
		o->oMoveAngleYaw	  = RandomU16();
		o->oVelY		  = 8;
		o->oBlackSmokeBowserUnkF4 = o->header.gfx.scale[0];
	}
	o->oMoveAngleYaw += o->oAngleVelYaw * FRAME_RATE_SCALER;
	o->oPosY += o->oVelY * FRAME_RATE_SCALER;
}

void bhv_black_smoke_mario_loop(void)
{
	if(o->oTimer == 0)
	{
		obj_set_pos_relative(gMarioObject, 0, 0, -30.0f);
		o->oForwardVel	 = Randomf() * 2 + 0.5;
		o->oMoveAngleYaw = (gMarioObject->oMoveAngleYaw + 0x7000) + Randomf() * 8192.0f;
		o->oVelY	 = 8;
	}
	o->oMoveAngleYaw += o->oAngleVelYaw * FRAME_RATE_SCALER;
	o->oPosY += o->oVelY * FRAME_RATE_SCALER;
}

void bhv_flame_mario_loop(void)
{
	s_set_scale(2.0f);
	if(o->oTimer != 0)
		if(o->oTimer % (2 * FRAME_RATE_SCALER_INV) == 0)
			s_makeobj_nowpos(o, MODEL_BURN_SMOKE, sm64::bhv::bhvBlackSmokeMario());
	gMarioObject->prevObj = o; // weird?
	set_object_parent_relative_pos(o, 40, -120, 0);
	if(!(gMarioObject->oMarioParticleFlags & 0x800))
	{
		o->parentObj->oActiveParticleFlags &= ~0x800;
		s_remove_obj(o);
		gMarioObject->prevObj = NULL;
	}
}
