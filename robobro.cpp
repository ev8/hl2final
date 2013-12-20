//ev8 this is the entity class for my robot helper
#include "cbase.h"
 
class CRobobro : public CBaseAnimating
{
public:
	DECLARE_CLASS( CRobobro, CBaseAnimating );
	DECLARE_DATADESC();
 
	CRobobro()
	{
		m_bActive = false;
	}
	//simple boiler plate entity declarations
	void Spawn( void );
	void Precache( void );
	void Activate(void);
	void MoveThink( void );
 
	// Input function
	void InputToggle( inputdata_t &inputData );
 
private:
 
	bool	m_bActive;
	float	m_flNextChangeTime;
};
 
LINK_ENTITY_TO_CLASS( Robro, CRobobro );
 
// Start of our data description for the class
BEGIN_DATADESC( CRobobro )
 
	// Save/restore our active state
	DEFINE_FIELD( m_bActive, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flNextChangeTime, FIELD_TIME ),
 
	// Links our input name from Hammer to our input member function
	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", InputToggle ),
 
	// Declare our think function
	DEFINE_THINKFUNC( MoveThink ),
 
END_DATADESC()
 
// Name of our entity's model
#define	ENTITY_MODEL	"models/combine_scanner.mdl"
 
//-----------------------------------------------------------------------------
// Purpose: Precache assets used by the entity
//-----------------------------------------------------------------------------
//ev8 loads the model, i'll add sounds later
void CRobobro::Precache( void )
{
	PrecacheModel( ENTITY_MODEL );
 
	BaseClass::Precache();
}
// ev8 boiler plate function for when entity is loaded,
 void CRobobro::Activate()
 {
	 SetThink( &CRobobro::MoveThink );
	 CBaseAnimating::Activate();
 }
//-----------------------------------------------------------------------------
// Purpose: Sets up the entity's initial state
//-----------------------------------------------------------------------------
// ev8 spawns my entity and gives it a bounding box for world collisions but not physics,
 // apparently getting something to fly and respond to physical touchs is very hard to do
 void CRobobro::Spawn( void )
{
	Precache();
 
	SetModel( ENTITY_MODEL );
	SetSolid( SOLID_BBOX );
	UTIL_SetSize( this, -Vector(20,20,20), Vector(20,20,20) );
	SetSequence( LookupSequence("idle") );
	// Set the animation speed to 100%
	SetPlaybackRate( 1.0f );
	// Tell the client to animate this model
	UseClientSideAnimation();
	SetThink( &CRobobro::MoveThink );
	SetNextThink( gpGlobals->curtime + 0.05f );
 
		// Start moving
		SetMoveType( MOVETYPE_FLY );
 


}
 
//-----------------------------------------------------------------------------
// Purpose: Think function to randomly move the entity
//-----------------------------------------------------------------------------
// ev8 my think function that make robobro follow the player that spawned him
void CRobobro::MoveThink( void )
{
	
	Vector	vTargetDir;
	VectorSubtract( this->GetOwnerEntity()->GetAbsOrigin(), GetAbsOrigin(), vTargetDir );
	Vector	vDir	= GetAbsVelocity();
	float	flSpeed	= VectorNormalize( vDir );
	Vector	vNewVelocity = vDir;
	
	
			vNewVelocity = vTargetDir;
	

	QAngle	finalAngles;
	VectorAngles( vNewVelocity, finalAngles );
	SetAbsAngles( finalAngles );

	vNewVelocity *= flSpeed;
	SetAbsVelocity( vNewVelocity );
 
	// Think every 20Hz
	SetNextThink( gpGlobals->curtime + 0.05f );
}
 
//-----------------------------------------------------------------------------
// Purpose: Toggle the movement of the entity
//-----------------------------------------------------------------------------
void CRobobro::InputToggle( inputdata_t &inputData )
{
	/*// Toggle our active state
	if ( !m_bActive )
	{
		// Start thinking
		SetThink( &CRobobro::MoveThink );
 
		SetNextThink( gpGlobals->curtime + 0.05f );
 
		// Start moving
		SetMoveType( MOVETYPE_FLY );
 
		// Force MoveThink() to choose a new speed and direction immediately
		m_flNextChangeTime = gpGlobals->curtime;
 
		// Update m_bActive to reflect our new state
		m_bActive = true;
	}
	else
	{
		// Stop thinking
		SetThink( NULL );
 
		// Stop moving
		SetAbsVelocity( vec3_origin );
 		SetMoveType( MOVETYPE_NONE );
 
		m_bActive = false;
	}*/
}

// ev8 a console command to spawn robobro, i'll move the spaning code to the player spawn code later, so the player and robobro spawn simultaniuly.
CON_COMMAND(create_Robro, "Creates a roBro")
{
	Vector vecForward;
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if(!pPlayer)
	{
		Warning("Could not determine calling player!\n");
		return;
	}
 
	AngleVectors( pPlayer->EyeAngles(), &vecForward );
	CBaseEntity *pEnt = CreateEntityByName( "Robro" );
	if ( pEnt )
	{
		pEnt->SetOwnerEntity(pPlayer);
		Vector vecOrigin = pPlayer->GetAbsOrigin() + vecForward * 256 + Vector(0,0,64);
		QAngle vecAngles(0, pPlayer->GetAbsAngles().y - 90, 0);
		pEnt->SetAbsOrigin(vecOrigin);
		pEnt->SetAbsAngles(vecAngles);
		DispatchSpawn(pEnt);
	}
}

