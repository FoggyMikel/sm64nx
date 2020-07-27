// grand_star.c.inc

s32 func_802B2894(Vec3f a0, Vec3f a1, f32 yVel, f32 gravity)
{
	f32 dx	       = a0[0] - a1[0];
	f32 dz	       = a0[2] - a1[2];
	f32 planarDist = sqrtf(dx * dx + dz * dz);
	s32 time;
	o->oMoveAngleYaw = atan2s(dz, dx);
	o->oVelY	 = yVel;
	o->oGravity	 = gravity;
	time		 = -2.0f / o->oGravity * yVel - 1.0f;
	o->oForwardVel	 = planarDist / time;
	return time;
}

void func_802B29B0(void)
{
	o->oGravity    = 0.0f;
	o->oVelY       = 0.0f;
	o->oForwardVel = 0.0f;
}

void bhv_grand_star_loop(void)
{
	UNUSED s32 unused;
	Vec3f sp28;
	sp28[0] = sp28[1] = sp28[2] = 0.0f;
	if(o->oAction == 0) // grand star spawn
	{
		if(o->oTimer == 0)
		{
			set_object_angle(o, 0, 0, 0);
			o->oAngleVelYaw = 0x400;
			objsound(SOUND_GENERAL2_STAR_APPEARS);
		}
		if(o->oTimer > 70 * FRAME_RATE_SCALER_INV)
			o->oAction++;
		spawn_sparkle_particles(3, 200, 80, -60); // spawns a bunch of glittery stars around the grand star
	}
	else if(o->oAction == 1) // star intro is finished and is interactable
	{
		if(o->oTimer == 0)
		{
			objsound(SOUND_GENERAL_GRAND_STAR);
			cutscene_object(CUTSCENE_STAR_SPAWN, o);
			o->oGrandStarUnk108 = func_802B2894(sp28, &o->oPosX, 80.0f, -2.0f);
		}

		s_optionmove_F();

		if(o->oSubAction == 0)
		{
			if(o->oPosY < o->oHomeY)
			{
				o->oPosY       = o->oHomeY;
				o->oVelY       = 60.0f;
				o->oForwardVel = 0.0f;
				o->oSubAction++;
				objsound(SOUND_GENERAL_GRAND_STAR_JUMP);
			}
		}
		else if(o->oVelY < 0.0f && o->oPosY < o->oHomeY + 200.0f)
		{
			o->oPosY = o->oHomeY + 200.0f;
			func_802B29B0();
			gObjCutsceneDone = 1;
			set_mario_npc_dialog(0);
			o->oAction++;
			o->oInteractStatus = 0;
			objsound(SOUND_GENERAL_GRAND_STAR_JUMP);
		}
		spawn_sparkle_particles(3, 200, 80, -60);
	}
	else // now can can interact
	{
		s_hitON();
		if(o->oInteractStatus & INT_STATUS_INTERACTED)
		{
			s_remove_obj(o);
			o->oInteractStatus = 0;
		}
	}
	if(o->oAngleVelYaw > 0x400)
		o->oAngleVelYaw -= 0x100 / FRAME_RATE_SCALER_INV;

	o->oFaceAngleYaw += o->oAngleVelYaw * FRAME_RATE_SCALER;
	s_set_scale(2.0f);
	o->oGraphYOffset = 110.0f;
}
