// mr_i.c.inc

// this is actually the MrI particle loop function. piranha
// plant code later on reuses this function.
void bhv_piranha_particle_loop(void)
{
	if(o->oTimer == 0)
	{
		o->oVelY	 = 20.0f + 20.0f * Randomf();
		o->oForwardVel	 = 20.0f + 20.0f * Randomf();
		o->oMoveAngleYaw = RandomU16();
	}
	s_optionmove_F();
}

void ActionMrIParticle0(void)
{
	s_set_scale(3.0f);
	o->oForwardVel = 20.0f;
	s_enemybgcheck();
	if(0x8000 & o->oInteractStatus)
		o->oAction = 1;
	else if((o->oTimer >= 101 * FRAME_RATE_SCALER_INV) || (0x200 & o->oMoveFlags) || (8 & (s16)o->activeFlags))
	{
		s_remove_obj(o);
		s_kemuri();
	}
}

void ActionMrIParticle1(void)
{
	s32 i;
	s_remove_obj(o);
	for(i = 0; i < 10; i++)
		s_makeobj_nowpos(o, MODEL_PURPLE_MARBLE, sm64::bhv::bhvPurpleParticle());
}

void (*sMrIParticleActions[])(void) = {ActionMrIParticle0, ActionMrIParticle1};

void bhv_mr_i_particle_loop(void)
{
	s_modejmp(sMrIParticleActions);
}

void func_802A525C(void)
{
	struct Object* particle;
	f32 sp18 = o->header.gfx.scale[1];
	particle = s_makeobj_nowpos(o, MODEL_PURPLE_MARBLE, sm64::bhv::bhvMrIParticle());
	particle->oPosY += 50.0f * sp18 * FRAME_RATE_SCALER;
	particle->oPosX += sins(o->oMoveAngleYaw) * 90.0f * sp18 * FRAME_RATE_SCALER;
	particle->oPosZ += coss(o->oMoveAngleYaw) * 90.0f * sp18 * FRAME_RATE_SCALER;
	objsound(SOUND_OBJ_MRI_SHOOT);
}

void bhv_mr_i_body_loop(void)
{
	s_copy_worldXYZ_angleXYZ(o, o->parentObj);
	if(!(8 & o->activeFlags))
	{
		s_copy_scale(o, o->parentObj);
		s_set_skeleton(o, 0, 0, o->header.gfx.scale[1] * 100.0f);
		s_create_matrix(o, 44, 15);
		s_rotate_vertex(o, 6, 44);
		o->oFaceAnglePitch = o->oMoveAnglePitch;
		o->oGraphYOffset   = o->header.gfx.scale[1] * 100.f;
	}

	if(o->parentObj->oMrIUnk110 != 1)
	{
		o->oAnimState = -1;
	}
	else
	{
		o->oAnimState++;

		if(o->oAnimState == 15)
		{
			o->parentObj->oMrIUnk110 = 0;
		}
	}
	if(!o->parentObj->activeFlags)
	{
		s_remove_obj(o);
	}
}

void ActionMrI3(void)
{
	s16 sp36;
	s16 sp34;
	f32 sp30;
	f32 sp2C;
	UNUSED u8 pad[8];
	f32 sp20;
	f32 sp1C;
	if(o->oBehParams2ndByte)
		sp1C = 2.0f;
	else
		sp1C = 1.0f;
	if(o->oMrIUnk100 < 0)
		sp34 = 0x1000;
	else
		sp34 = -0x1000;
	sp2C = (o->oTimer + 1) / (96.0f / FRAME_RATE_SCALER);

	if(o->oTimer < 64 * FRAME_RATE_SCALER_INV)
	{
		sp36 = o->oMoveAngleYaw;
		o->oMoveAngleYaw += (sp34 * coss(0x4000 * sp2C)) / FRAME_RATE_SCALER_INV;

		if(sp36 < 0 && o->oMoveAngleYaw >= 0)
			objsound(SOUND_OBJ2_MRI_SPINNING);
		o->oMoveAnglePitch = (1.0 - coss(0x4000 * sp2C)) * -0x4000;
		s_effect_updown(4.0f);
	}
	else if(o->oTimer < 96 * FRAME_RATE_SCALER_INV)
	{
		if(o->oTimer == 64 * FRAME_RATE_SCALER_INV)
			objsound(SOUND_OBJ_MRI_DEATH);
		sp30 = (f32)((o->oTimer / FRAME_RATE_SCALER_INV) - 63) / 32;
		o->oMoveAngleYaw += (sp34 * coss(0x4000 * sp2C)) / FRAME_RATE_SCALER_INV;
		o->oMoveAnglePitch = (1.0 - coss(0x4000 * sp2C)) * -0x4000;
		s_effect_updown((s32)((1.0f - sp30) * 4)); // trucating the f32?
		sp20 = coss(0x4000 * sp30) * 0.4 + 0.6;
		s_set_scale(sp20 * sp1C);
	}
	else if(o->oTimer < 104 * FRAME_RATE_SCALER_INV)
	{
		// do nothing
	}
	else if(o->oTimer < 168 * FRAME_RATE_SCALER_INV)
	{
		if(o->oTimer == 104 * FRAME_RATE_SCALER_INV)
		{
			s_hitOFF();
			s_kemuri();
			o->oMrISize = sp1C * 0.6;
			if(o->oBehParams2ndByte)
			{
				o->oPosY += 100.0f * FRAME_RATE_SCALER;
				s_enemyset_star(1370, 2000.0f, -320.0f);
				s_remove_obj(o);
			}
			else
				s_make_bluecoin();
		}
		o->oMrISize -= 0.2 * sp1C * FRAME_RATE_SCALER;
		if(o->oMrISize < 0)
			o->oMrISize = 0;
		s_set_scale(o->oMrISize);
	}
	else
		s_remove_obj(o);
}

void ActionMrI2()
{
	s16 yawStart;
	s16 yawVelocity;

	yawStart = o->oMoveAngleYaw;

	if(o->oTimer == 0)
	{
		if(o->oBehParams2ndByte)
		{
			o->oMrIUnkF4 = 200;
		}
		else
		{
			o->oMrIUnkF4 = 120;
		}

		o->oMrIUnkFC  = 0;
		o->oMrIUnk100 = 0;
		o->oMrIUnk104 = 0;
	}

	s_chase_obj_angle(o, gMarioObject, 0x10, 0x800 / FRAME_RATE_SCALER_INV);
	s_chase_obj_angle(o, gMarioObject, 0x0F, 0x400 / FRAME_RATE_SCALER_INV);

	yawVelocity = (yawStart - (s16)(o->oMoveAngleYaw)) * FRAME_RATE_SCALER_INV;

	if(!yawVelocity)
	{
		o->oMrIUnkFC  = 0;
		o->oMrIUnk100 = 0;
	}
	else if(yawVelocity > 0)
	{
		if(o->oMrIUnk100 > 0)
		{
			o->oMrIUnkFC += yawVelocity / FRAME_RATE_SCALER_INV;
		}
		else
		{
			o->oMrIUnkFC = 0;
		}

		o->oMrIUnk100 = 1;
	}
	else
	{
		if(o->oMrIUnk100 < 0)
		{
			o->oMrIUnkFC -= yawVelocity / FRAME_RATE_SCALER_INV;
		}
		else
		{
			o->oMrIUnkFC = 0;
		}

		o->oMrIUnk100 = -1;
	}

	if(!o->oMrIUnkFC)
	{
		o->oMrIUnkF4 = 120;
	}

	if(o->oMrIUnkFC > 1 << 16)
	{
		o->oAction = 3;
	}

	o->oMrIUnkF4 -= 1;

	if(!o->oMrIUnkF4)
	{
		o->oMrIUnkF4 = 120;
		o->oMrIUnkFC = 0;
	}

	if(o->oMrIUnkFC < 5000)
	{
		if(o->oMrIUnk104 == o->oMrIUnk108)
		{
			o->oMrIUnk110 = 1;
		}

		if(o->oMrIUnk104 == o->oMrIUnk108 + 20)
		{
			func_802A525C();
			o->oMrIUnk104 = 0;
			o->oMrIUnk108 = (s32)(Randomf() * 50.0f + 50.0f);
		}
		o->oMrIUnk104++;
	}
	else
	{
		o->oMrIUnk104 = 0;
		o->oMrIUnk108 = (s32)(Randomf() * 50.0f + 50.0f);
	}

	if(o->oDistanceToMario > 800.0f)
	{
		o->oAction = 1;
	}
}

void ActionMrI1(void)
{
	s16 sp1E;
	s16 sp1C;
	s16 sp1A;
	sp1E = s_calc_targetangle(o, gMarioObject);
	sp1C = s_calc_dangle(o->oMoveAngleYaw, sp1E);
	sp1A = s_calc_dangle(o->oMoveAngleYaw, gMarioObject->oFaceAngleYaw);
	if(o->oTimer == 0)
	{
		s_hitON();
		o->oMoveAnglePitch = 0;
		o->oMrIUnk104	   = 30;
		o->oMrIUnk108	   = Randomf() * 20.0f;
		if(o->oMrIUnk108 & 1)
			o->oAngleVelYaw = -256;
		else
			o->oAngleVelYaw = 256;
	}
	if(sp1C < 1024 && sp1A > 0x4000)
	{
		if(o->oDistanceToMario < 700.0f)
			o->oAction = 2;
		else
			o->oMrIUnk104++;
	}
	else
	{
		o->oMoveAngleYaw += o->oAngleVelYaw * FRAME_RATE_SCALER;
		o->oMrIUnk104 = 30;
	}
	if(o->oMrIUnk104 == o->oMrIUnk108 + 60)
		o->oMrIUnk110 = 1;
	if(o->oMrIUnk108 + 80 < o->oMrIUnk104)
	{
		o->oMrIUnk104 = 0;
		o->oMrIUnk108 = Randomf() * 80.0f;
		func_802A525C();
	}
}

void ActionMrI0(void)
{
	s_set_angle(o, 0, 0, 0);
	s_set_scale(o->oBehParams2ndByte + 1);
	if(o->oTimer == 0)
	{
		s_copy_initpos();
	}

	if(o->oDistanceToMario < 1500.0f)
	{
		o->oAction = 1;
	}
}

void (*sMrIActions[])(void) = {ActionMrI0, ActionMrI1, ActionMrI2, ActionMrI3};

struct ObjectHitbox sMrIHitbox = {
    /* interactType: */ INTERACT_DAMAGE,
    /* downOffset: */ 0,
    /* damageOrCoinValue: */ 2,
    /* health: */ 2,
    /* numLootCoins: */ 5,
    /* radius: */ 80,
    /* height: */ 150,
    /* hurtboxRadius: */ 0,
    /* hurtboxHeight: */ 0,
};

void bhv_mr_i_loop(void)
{
	s_set_hitparam(o, &sMrIHitbox);
	s_modejmp(sMrIActions);

	if(o->oAction != 3)
	{
		if(o->oDistanceToMario > 3000.0f || o->activeFlags & 8)
		{
			o->oAction = 0;
		}
	}
	o->oInteractStatus = 0;
}
