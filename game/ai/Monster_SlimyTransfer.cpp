
#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"

class rvMonsterSlimyTransfer : public idAI {
public:

	CLASS_PROTOTYPE( rvMonsterSlimyTransfer );

	rvMonsterSlimyTransfer ( void );

	void				InitSpawnArgsVariables			( void );
	void				Spawn							( void );
	void				Save							( idSaveGame *savefile ) const;
	void				Restore							( idRestoreGame *savefile );

protected:

	virtual bool		CheckActions					( void );
	virtual void		OnDeath							( void );

	jointHandle_t		jointVomitMuzzle;

	int					vomitNextAttackTime;
	int					vomitAttackRate;

private:

	rvAIAction			actionVomitAttack;
	
	// Torso States
	stateResult_t		State_Torso_VomitAttack			( const stateParms_t& parms );
	stateResult_t		State_Torso_FinishVomitAttack	( const stateParms_t& parms );

	// Frame commands
	stateResult_t		State_Frame_StartVomit			( const stateParms_t& parms );
	stateResult_t		State_Frame_StopVomit			( const stateParms_t& parms );

	CLASS_STATES_PROTOTYPE ( rvMonsterSlimyTransfer );
};

CLASS_DECLARATION( idAI, rvMonsterSlimyTransfer )
END_CLASS

/*
================
rvMonsterSlimyTransfer::rvMonsterSlimyTransfer
================
*/
rvMonsterSlimyTransfer::rvMonsterSlimyTransfer ( void ) {
}

void rvMonsterSlimyTransfer::InitSpawnArgsVariables ( void ) {
	jointVomitMuzzle = animator.GetJointHandle ( spawnArgs.GetString ( "joint_vomitMuzzle", "puke_bone" ) );
	
	vomitAttackRate = SEC2MS ( spawnArgs.GetFloat ( "attack_vomit_rate", ".15" ) );
}

/*
================
rvMonsterSlimyTransfer::Spawn
================
*/
void rvMonsterSlimyTransfer::Spawn ( void ) {
	actionVomitAttack.Init ( spawnArgs, "action_vomitAttack", "Torso_VomitAttack", AIACTIONF_ATTACK );

	InitSpawnArgsVariables();
}

/*
================
rvMonsterSlimyTransfer::Save
================
*/
void rvMonsterSlimyTransfer::Save ( idSaveGame *savefile ) const {
	savefile->WriteInt( vomitNextAttackTime );

	actionVomitAttack.Save( savefile );
}

/*
================
rvMonsterSlimyTransfer::Restore
================
*/
void rvMonsterSlimyTransfer::Restore ( idRestoreGame *savefile ) {
	savefile->ReadInt( vomitNextAttackTime );

	actionVomitAttack.Restore( savefile );

	InitSpawnArgsVariables();
}

/*
================
rvMonsterSlimyTransfer::CheckActions
================
*/
bool rvMonsterSlimyTransfer::CheckActions ( void ) {
	if ( PerformAction ( &actionVomitAttack, (checkAction_t)&idAI::CheckAction_RangedAttack, &actionTimerRangedAttack ) ) {
		return true;
	}
	return idAI::CheckActions ( );
}

/*
================
rvMonsterSlimyTransfer::OnDeath
================
*/
void rvMonsterSlimyTransfer::OnDeath ( void ) {
	StopEffect ( "fx_vomit_muzzle" );


		const char *value;
		idEntity* dropitem = NULL;
		idDict	  args;


		int itemDropped = rand() % 14;
		switch (itemDropped){
		case 0:
			value = "weapon_blaster";
			break;
		case 1:
			value = "weapon_dmg";
			break;
		case 2:
			value = "weapon_grenadelauncher";
			break;
		case 3:
			value = "weapon_hyperblaster";
			break;
		case 4:
			value = "weapon_lightninggun";
			break;
		case 5:
			value = "weapon_nailgun";
			break;
		case 6:
			value = "weapon_napalmgun";
			break;
		case 7:
			value = "weapon_railgun";
			break;
		case 8:
			value = "weapon_rocketlauncher";
			break;
		case 9:
			value = "weapon_shotgun";
			break;
		case 10:
			value = "powerup_quad_damage";
			break;
		case 11:
			value = "powerup_haste";
			break;
		case 12:
			value = "powerup_regeneration";
			break;
		case 13:
			value = "powerup_invisibility";
			break;
		case 14:
			value = "powerup_team_damage_mod";
			break;
		}


		args.Set("classname", value);
		args.SetVector("origin", GetPhysics()->GetOrigin());
		args.SetInt("angle", move.current_yaw);
		gameLocal.SpawnEntityDef(args, &dropitem);

		gameLocal.GetLocalPlayer()->ded = true;
		gameLocal.GetLocalPlayer()->startdrop = gameLocal.GetTime();

	idAI::OnDeath ( );
}


/*
===============================================================================

	States 

===============================================================================
*/

CLASS_STATES_DECLARATION ( rvMonsterSlimyTransfer )
	STATE ( "Torso_VomitAttack",		rvMonsterSlimyTransfer::State_Torso_VomitAttack )
	STATE ( "Torso_FinishVomitAttack",	rvMonsterSlimyTransfer::State_Torso_FinishVomitAttack )

	STATE ( "Frame_StartVomit",			rvMonsterSlimyTransfer::State_Frame_StartVomit )
	STATE ( "Frame_StopVomit",			rvMonsterSlimyTransfer::State_Frame_StopVomit )
END_CLASS_STATES

/*
================
rvMonsterSlimyTransfer::State_Torso_VomitAttack
================
*/
stateResult_t rvMonsterSlimyTransfer::State_Torso_VomitAttack ( const stateParms_t& parms ) {	
	enum {
		STAGE_INIT,
		STAGE_WAIT,
	};	
	switch ( parms.stage ) {
		case STAGE_INIT:
			DisableAnimState ( ANIMCHANNEL_LEGS );

			vomitNextAttackTime = 0;

			// Loop the flame animation
			PlayAnim( ANIMCHANNEL_TORSO, "vomit_attack", parms.blendFrames );

			// Make sure we clean up some things when this state is finished (effects for one)			
			PostAnimState ( ANIMCHANNEL_TORSO, "Torso_FinishVomitAttack", 0, 0, SFLAG_ONCLEAR );
			
			return SRESULT_STAGE ( STAGE_WAIT );
		
		case STAGE_WAIT:
			if ( AnimDone ( ANIMCHANNEL_TORSO, parms.blendFrames ) ) {
				return SRESULT_DONE;
			}
			
			if ( vomitNextAttackTime && gameLocal.time >= vomitNextAttackTime ) {
				Attack ( "vomit", jointVomitMuzzle, enemy.ent );
				vomitNextAttackTime = gameLocal.time + vomitAttackRate;
			}
			
			return SRESULT_WAIT;			
	}

	return SRESULT_ERROR;
}

/*
================
rvMonsterSlimyTransfer::State_Torso_FinishVomitAttack
================
*/
stateResult_t rvMonsterSlimyTransfer::State_Torso_FinishVomitAttack ( const stateParms_t& parms ) {	
	State_Frame_StopVomit ( parms );
	return SRESULT_DONE;
}

/*
================
rvMonsterSlimyTransfer::State_Frame_StartVomit
================
*/
stateResult_t rvMonsterSlimyTransfer::State_Frame_StartVomit ( const stateParms_t& parms ) {	
	PlayEffect ( "fx_vomit_muzzle", jointVomitMuzzle, true );
	vomitNextAttackTime = gameLocal.time;	
	return SRESULT_DONE;
}

/*
================
rvMonsterSlimyTransfer::State_Frame_StopVomit
================
*/
stateResult_t rvMonsterSlimyTransfer::State_Frame_StopVomit ( const stateParms_t& parms ) {	
	StopEffect ( "fx_vomit_muzzle" );
	vomitNextAttackTime = 0;
	return SRESULT_DONE;
}
