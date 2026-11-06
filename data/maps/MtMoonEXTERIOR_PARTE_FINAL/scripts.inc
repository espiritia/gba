MtMoonEXTERIOR_PARTE_FINAL_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMoonEXTERIOR_PARTE_FINAL_OnTransition
	.byte 0

MtMoonEXTERIOR_PARTE_FINAL_OnTransition::
	special MinerGame_Reset
	call_if_set FLAG_MT_MOON_MINER3_BEATEN, MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerAside
	end

MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerAside::
	setobjectxyperm LOCALID_MT_MOON_MINER_3, 26, 20
	setobjectmovementtype LOCALID_MT_MOON_MINER_3, MOVEMENT_TYPE_FACE_DOWN
	return

MtMoonEXTERIOR_PARTE_FINAL_EventScript_ExitToCave::
	lockall
	warp MAP_MT_MOON_PARTE_1, 25, 21
	waitstate
	releaseall
	end

@ ---------------------------------------------------------------
@ Tercer hermano minero: minijuego ROMPEPIEDRAS
@ ---------------------------------------------------------------
MtMoonEXTERIOR_PARTE_FINAL_EventScript_Miner::
	@ Con el minijuego en marcha no se puede volver a hablar con él
	specialvar VAR_RESULT, MinerGame_IsActive
	goto_if_eq VAR_RESULT, TRUE, MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerBusy
	lock
	faceplayer
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_MinerMotto
	goto_if_set FLAG_MT_MOON_MINER3_BEATEN, MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerAlreadyBeaten
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_MinerIntro
	call MtMoonEXTERIOR_PARTE_FINAL_EventScript_SendOutMachop
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_MinerGivePickaxe
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_KEY_ITEM
	message MtMoonEXTERIOR_PARTE_FINAL_Text_ReceivedPickaxe
	waitmessage
	waitfanfare
	textcolor NPC_TEXT_COLOR_MALE
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_MinerRules
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_MinerReady, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerNotReady
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_MinerCountdown
	closemessage
	applymovement LOCALID_MT_MOON_MINER_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	playbgm MUS_POKE_JUMP, FALSE
	special MinerGame_Start
	release
	end

MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerNotReady::
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_MinerNotReady
	call MtMoonEXTERIOR_PARTE_FINAL_EventScript_ReturnMachop
	release
	end

@ "¡Adelante, MACHOP!": aparece encima del minero (o debajo si el jugador
@ está ahí)
MtMoonEXTERIOR_PARTE_FINAL_EventScript_SendOutMachop::
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_GoMachop
	closemessage
	specialvar VAR_RESULT, MinerGame_IsPlayerAboveMiner
	call_if_eq VAR_RESULT, FALSE, MtMoonEXTERIOR_PARTE_FINAL_EventScript_MachopAbove
	call_if_eq VAR_RESULT, TRUE, MtMoonEXTERIOR_PARTE_FINAL_EventScript_MachopBelow
	playse SE_BALL_OPEN
	addobject LOCALID_MINER_GAME_MACHOP
	waitse
	playmoncry SPECIES_MACHOP, CRY_MODE_NORMAL
	waitmoncry
	return

MtMoonEXTERIOR_PARTE_FINAL_EventScript_MachopAbove::
	setobjectxyperm LOCALID_MINER_GAME_MACHOP, 26, 20
	return

MtMoonEXTERIOR_PARTE_FINAL_EventScript_MachopBelow::
	setobjectxyperm LOCALID_MINER_GAME_MACHOP, 26, 22
	return

MtMoonEXTERIOR_PARTE_FINAL_EventScript_ReturnMachop::
	playse SE_BALL_OPEN
	removeobject LOCALID_MINER_GAME_MACHOP
	waitse
	return

@ Hablar con MACHOP
MtMoonEXTERIOR_PARTE_FINAL_EventScript_Machop::
	lock
	playmoncry SPECIES_MACHOP, CRY_MODE_NORMAL
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_Machop
	waitmoncry
	release
	end

MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerAlreadyBeaten::
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_MinerPostGame
	release
	end

@ Lo que pasa al pulsar A sobre una piedra (o señuelo)
MtMoonEXTERIOR_PARTE_FINAL_EventScript_GameRock::
	special MinerGame_HitObject
	goto_if_eq VAR_RESULT, 2, MtMoonEXTERIOR_PARTE_FINAL_EventScript_GameDecoyHit
	end

MtMoonEXTERIOR_PARTE_FINAL_EventScript_GameDecoyHit::
	special MinerGame_RemoveObject
	playse SE_FAILURE
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_QuestionMark
	waitmovement 0
	delay 30
	end

@ Lanzados desde src/miner_game.c al terminar el minijuego
MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerBusy::
	end

MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerGameLeftArea::
	lockall
	fadedefaultbgm
	playse SE_FAILURE
	applymovement LOCALID_MT_MOON_MINER_3, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_MinerYouLeft
	goto MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerGameLostEnd

MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerGameLost::
	lockall
	fadedefaultbgm
	playse SE_FAILURE
	applymovement LOCALID_MT_MOON_MINER_3, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_MinerYouLost
MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerGameLostEnd::
	closemessage
	special MinerGame_ClearAll
	call MtMoonEXTERIOR_PARTE_FINAL_EventScript_ReturnMachop
	releaseall
	end

MtMoonEXTERIOR_PARTE_FINAL_EventScript_MinerGameWon::
	lockall
	special MinerGame_ClearAll
	fadedefaultbgm
	playfanfare MUS_LEVEL_UP
	waitfanfare
	applymovement LOCALID_MT_MOON_MINER_3, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MtMoonEXTERIOR_PARTE_FINAL_Text_MinerYouWon
	closemessage
	call MtMoonEXTERIOR_PARTE_FINAL_EventScript_ReturnMachop
	specialvar VAR_RESULT, MinerGame_IsPlayerAboveMiner
	call_if_eq VAR_RESULT, TRUE, MtMoonEXTERIOR_PARTE_FINAL_EventScript_PlayerMakeRoom
	applymovement LOCALID_MT_MOON_MINER_3, MtMoonEXTERIOR_PARTE_FINAL_Movement_MinerStepAside
	waitmovement 0
	setobjectxyperm LOCALID_MT_MOON_MINER_3, 26, 20
	setobjectmovementtype LOCALID_MT_MOON_MINER_3, MOVEMENT_TYPE_FACE_DOWN
	setflag FLAG_MT_MOON_MINER3_BEATEN
	releaseall
	end

MtMoonEXTERIOR_PARTE_FINAL_EventScript_PlayerMakeRoom::
	applymovement OBJ_EVENT_ID_PLAYER, MtMoonEXTERIOR_PARTE_FINAL_Movement_PlayerMakeRoom
	waitmovement 0
	return

MtMoonEXTERIOR_PARTE_FINAL_Movement_MinerStepAside::
	walk_up
	face_down
	step_end

MtMoonEXTERIOR_PARTE_FINAL_Movement_PlayerMakeRoom::
	walk_left
	face_right
	step_end
