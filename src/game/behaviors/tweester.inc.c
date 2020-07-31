// tweester.c.inc

struct ObjectHitbox sTweesterHitbox = {
    /* interactType: */ INTERACT_TORNADO,
    /* downOffset: */ 0,
    /* damageOrCoinValue: */ 0,
    /* health: */ 0,
    /* numLootCoins: */ 0,
    /* radius: */ 1500,
    /* height: */ 4000,
    /* hurtboxRadius: */ 0,
    /* hurtboxHeight: */ 0,
};

void func_802C231C(f32 a0)
{
	s16 sp6		       = 0x2C00;
	f32 sp0		       = a0 * 0.4;
	o->header.gfx.scale[0] = (((coss(o->oTweesterUnkF4) + 1.0) * 0.5 * 0.3) + 1.0) * sp0;
	o->header.gfx.scale[1] = (((-coss(o->oTweesterUnkF4) + 1.0) * 0.5 * 0.5) + 0.5) * sp0;
	o->header.gfx.scale[2] = (((coss(o->oTweesterUnkF4) + 1.0) * 0.5 * 0.3) + 1.0) * sp0;
	o->oTweesterUnkF4 += 0x200 / FRAME_RATE_SCALER_INV;
	o->oForwardVel = 14.0f;
	o->oFaceAngleYaw += sp6 / FRAME_RATE_SCALER_INV;
}

void ActionTweester0(void)
{
	if(o->oSubAction == 0)
	{
		s_hitON();
		s_copy_initpos();
		s_set_scale(0);
		o->oTweesterUnkF8 = 0;
		if(o->oDistanceToMario < 1500.0f)
			o->oSubAction++;
		o->oTimer = 0;
	}
	else
	{
		objsound_level(SOUND_ENV_WIND1);
		func_802C231C(o->oTimer / (60.0f / FRAME_RATE_SCALER));
		if(o->oTimer > 59 * FRAME_RATE_SCALER_INV)
			o->oAction = 1;
	}
}

void ActionTweester1(void)
{
	f32 sp1C   = o->oBehParams2ndByte * 0x64;
	o->oUnk1BC = s_calc_returnangle();
	objsound_level(SOUND_ENV_WIND1);
	if(s_calc_playerscope() < sp1C && o->oSubAction == 0)
	{
		o->oForwardVel = 20.0f;
		s_chase_angleY(o->oAngleToMario, 0x200 / FRAME_RATE_SCALER_INV);

		if(playerWorks->status == ACT_TWIRLING)
			o->oSubAction++;
	}
	else
	{
		o->oForwardVel = 20.0f;
		s_chase_angleY(o->oUnk1BC, 0x200 / FRAME_RATE_SCALER_INV);
		if(s_calc_enemyscope() < 200.0f)
			o->oAction = 2;
	}
	if(o->oDistanceToMario > 3000.0f)
		o->oAction = 2;
	s_enemybgcheck();
	if(o->oMoveFlags & 0x200)
		o->oMoveAngleYaw = o->oWallAngle;
	s_enemymove(60);
	func_802C231C(1.0f);
	s_makeobj_nowpos(o, MODEL_SAND_DUST, sm64::bhv::bhvTornadoSandParticle());
}

void ActionTweester2(void)
{
	f32 sp1C = (60.0f / FRAME_RATE_SCALER) - o->oTimer;
	if(sp1C >= 0.0f)
		func_802C231C(sp1C / 60.0f);
	else
	{
		s_hitOFF();
		if(s_calc_playerscope() > 2500.0f)
			o->oAction = 0;
		if(o->oTimer > 360 * FRAME_RATE_SCALER_INV)
			o->oAction = 0;
	}
}

void (*sTweesterActions[])(void) = {ActionTweester0, ActionTweester1, ActionTweester2};

void bhv_tweester_loop(void)
{
	s_set_hitparam(o, &sTweesterHitbox);
	s_modejmp(sTweesterActions);
	o->oInteractStatus = 0;
}

void bhv_tweester_sand_particle_loop(void)
{
	o->oMoveAngleYaw += 0x3700 / FRAME_RATE_SCALER_INV;
	o->oForwardVel += 15.0f * FRAME_RATE_SCALER;
	o->oPosY += 22.0f * FRAME_RATE_SCALER;
	s_set_scale(Randomf() + 1.0);
	if(o->oTimer == 0)
	{
		s_random_XZ_offset(o, 100.0f);
		o->oFaceAnglePitch = RandomU16();
		o->oFaceAngleYaw   = RandomU16();
	}
	if(o->oTimer > 15 * FRAME_RATE_SCALER_INV)
		s_remove_obj(o);
}
