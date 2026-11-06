MT_MOON_PARTE_1_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MT_MOON_PARTE_1_OnTransition
	.byte 0

MT_MOON_PARTE_1_OnTransition::
	call_if_defeated TRAINER_MT_MOON_MINER, MT_MOON_PARTE_1_EventScript_MinerAside
	end

MT_MOON_PARTE_1_EventScript_MinerAside::
	setobjectxyperm LOCALID_MT_MOON_MINER_2, 25, 20
	setobjectmovementtype LOCALID_MT_MOON_MINER_2, MOVEMENT_TYPE_FACE_DOWN
	return

@ Segundo hermano minero (25,21): combate con equipo aleatorio (CreateMtMoonMinerParty).
@ Si le vences se aparta a (25,20) y deja libre el paso al este.
MT_MOON_PARTE_1_EventScript_Miner::
	trainerbattle_single TRAINER_MT_MOON_MINER, MT_MOON_PARTE_1_Text_MinerIntro, MT_MOON_PARTE_1_Text_MinerDefeat, MT_MOON_PARTE_1_EventScript_MinerDefeated
	msgbox MT_MOON_PARTE_1_Text_MinerPostBattle, MSGBOX_AUTOCLOSE
	end

MT_MOON_PARTE_1_EventScript_MinerDefeated::
	msgbox MT_MOON_PARTE_1_Text_MinerLetsYouPass
	closemessage
	call_if_eq VAR_FACING, DIR_SOUTH, MT_MOON_PARTE_1_EventScript_MinerPlayerStepAside
	applymovement LOCALID_MT_MOON_MINER_2, MT_MOON_PARTE_1_Movement_MinerStepAside
	waitmovement 0
	setobjectxyperm LOCALID_MT_MOON_MINER_2, 25, 20
	setobjectmovementtype LOCALID_MT_MOON_MINER_2, MOVEMENT_TYPE_FACE_DOWN
	release
	end

@ El jugador le habla desde (25,20): se aparta a la izquierda para dejarle sitio
MT_MOON_PARTE_1_EventScript_MinerPlayerStepAside::
	applymovement LOCALID_PLAYER, MT_MOON_PARTE_1_Movement_PlayerStepAside
	waitmovement 0
	return

MT_MOON_PARTE_1_Movement_MinerStepAside::
	walk_up
	face_down
	step_end

MT_MOON_PARTE_1_Movement_PlayerStepAside::
	walk_left
	face_right
	step_end

MT_MOON_PARTE_1_EventScript_ExitToExterior::
	lockall
	warp MAP_MT_MOON_EXTERIOR_PARTE_FINAL, 12, 17
	waitstate
	releaseall
	end
