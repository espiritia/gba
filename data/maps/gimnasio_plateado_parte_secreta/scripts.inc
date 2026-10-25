gimnasio_plateado_parte_secreta_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PewterCave_OnTransition
	.byte 0

@ Brock solo espera mientras no ha empezado la huida (al reintentar vuelve a estar)
PewterCave_OnTransition::
	call_if_eq VAR_PEWTER_CAVE_STATE, 0, PewterCave_EventScript_ShowBrock
	call_if_ne VAR_PEWTER_CAVE_STATE, 0, PewterCave_EventScript_HideBrock
	@ Huida en curso al cargar la partida: el motor borra la música guardada antes de este script
	call_if_eq VAR_PEWTER_CAVE_STATE, 1, PewterCave_EventScript_RestoreEscape
	end

PewterCave_EventScript_RestoreEscape::
	savebgm MUS_SILPH
	return

PewterCave_EventScript_ShowBrock::
	clearflag FLAG_HIDE_PEWTER_CAVE_BROCK
	return

PewterCave_EventScript_HideBrock::
	setflag FLAG_HIDE_PEWTER_CAVE_BROCK
	return

@ Etapa 1: Brock ve al jugador al cruzar la columna x12 de la sala de entrada
PewterCave_EventScript_BrockSeesPlayer::
	lockall
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	playse SE_PIN
	applymovement LOCALID_PEWTER_CAVE_BROCK, Common_Movement_ExclamationMark
	waitmovement 0
	playbgm MUS_FOLLOW_ME, FALSE
	applymovement LOCALID_PEWTER_CAVE_BROCK, Common_Movement_Delay48
	waitmovement 0
	@ El jugador se coloca en la fila de Brock (y5) y Brock se acerca hasta (13,5)
	getplayerxy VAR_0x8008, VAR_0x8009
	call_if_lt VAR_0x8009, 5, PewterCave_EventScript_AlignDown
	call_if_gt VAR_0x8009, 5, PewterCave_EventScript_AlignUp
	turnobject LOCALID_PLAYER, DIR_EAST
	applymovement LOCALID_PEWTER_CAVE_BROCK, PewterCave_Movement_BrockApproach
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox PewterCave_Text_BrockGreeting, MSGBOX_DEFAULT
	@ Al repetir la cueva tras el derrumbe no vuelve a dar el GEODUDE
	goto_if_set FLAG_GOT_BROCK_GEODUDE, PewterCave_EventScript_BrockRemindStrength
	msgbox PewterCave_Text_BrockGivesGeodude, MSGBOX_DEFAULT
	special GiveBrockGeodude
	goto_if_eq VAR_RESULT, MON_CANT_GIVE, PewterCave_EventScript_BrockLeave
	setflag FLAG_GOT_BROCK_GEODUDE
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message PewterCave_Text_ReceivedGeodude
	waitmessage
	waitfanfare
	call_if_eq VAR_RESULT, MON_GIVEN_TO_PC, PewterCave_EventScript_GeodudeSentToPC
	goto PewterCave_EventScript_BrockLeave
	end

PewterCave_EventScript_GeodudeSentToPC::
	msgbox PewterCave_Text_GeodudeSentToPC, MSGBOX_DEFAULT
	return

PewterCave_EventScript_BrockRemindStrength::
	msgbox PewterCave_Text_BrockRemindStrength, MSGBOX_DEFAULT
	goto PewterCave_EventScript_BrockLeave
	end

@ Etapa 2: Brock se adelanta hacia el este y empieza el minuto (oculto) para llegar a la salida
PewterCave_EventScript_BrockLeave::
	textcolor NPC_TEXT_COLOR_MALE
	msgbox PewterCave_Text_BrockWillWait, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PEWTER_CAVE_BROCK, PewterCave_Movement_BrockLeave
	waitmovement 0
	removeobject LOCALID_PEWTER_CAVE_BROCK
	@ Empieza la huida: música de urgencia (guardada para después de los combates)
	playbgm MUS_SILPH, TRUE
	setvar VAR_PEWTER_CAVE_STATE, 1
	special StartPewterCaveTimer
	releaseall
	end

PewterCave_EventScript_AlignDown::
	applymovement LOCALID_PLAYER, PewterCave_Movement_PlayerDown
	waitmovement 0
	addvar VAR_0x8009, 1
	goto_if_lt VAR_0x8009, 5, PewterCave_EventScript_AlignDown
	return

PewterCave_EventScript_AlignUp::
	applymovement LOCALID_PLAYER, PewterCave_Movement_PlayerUp
	waitmovement 0
	subvar VAR_0x8009, 1
	goto_if_gt VAR_0x8009, 5, PewterCave_EventScript_AlignUp
	return

@ Etapa 3: llega a la salida (26,29) a tiempo
PewterCave_EventScript_ReachExit::
	lockall
	setvar VAR_PEWTER_CAVE_STATE, 2
	@ Destino provisional: cambiarlo cuando exista la conexión definitiva de la salida
	warp MAP_PARTE_SECRETA_GIMNASIO_PLATEADO, 13, 3
	waitstate
	releaseall
	end

@ Se acabó el minuto (lo lanza TryRunPewterCaveTimeout en C): el techo se derrumba
@ y se vuelve al principio de la cueva, con Brock esperando otra vez
PewterCave_EventScript_TimeUp::
	lockall
	call ViridianForest_EventScript_LugiaShake
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PewterCave_Text_RoofCollapses, MSGBOX_DEFAULT
	closemessage
	call ViridianForest_EventScript_LugiaShake
	setvar VAR_PEWTER_CAVE_STATE, 0
	warp MAP_GIMNASIO_PLATEADO_PARTE_SECRETA, 5, 4
	waitstate
	releaseall
	end

@ Cada 10-15 s de huida (lo lanza TryRunPewterCaveTimeout en C): temblor y caen escombros
PewterCave_EventScript_Quake::
	lockall
	playse SE_M_ROCK_THROW
	special SpawnCaveDebrisDust
	setvar VAR_0x8004, 2  @ Vertical pan
	setvar VAR_0x8005, 3  @ Horizontal pan
	setvar VAR_0x8006, 6  @ Num shakes
	setvar VAR_0x8007, 2  @ Shake delay
	special ShakeScreen
	delay 16
	playse SE_M_ROCK_THROW
	special SpawnCaveDebrisDust
	delay 20
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PewterCave_Text_MoreDebris, MSGBOX_DEFAULT
	closemessage
	releaseall
	end

@ Como EventScript_StrengthBoulder, pero sin pedir la 4ª medalla: FUERZA es la del GEODUDE de Brock
PewterCave_EventScript_Boulder::
	goto_if_questlog EventScript_ReleaseEnd
	lockall
	goto_if_set FLAG_SYS_USE_STRENGTH, EventScript_AlreadyUsedStrength
	checkpartymove MOVE_STRENGTH
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_CantMoveBoulder
	setfieldeffectargument 0, VAR_RESULT
	msgbox Text_UseStrength, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_DontUseStrength
	closemessage
	dofieldeffect FLDEFF_USE_STRENGTH
	waitstate
	goto EventScript_UseStrength
	end

PewterCave_EventScript_ItemPotion::
	finditem ITEM_POTION
	end

PewterCave_EventScript_ItemPokeBall::
	finditem ITEM_POKE_BALL
	end

PewterCave_EventScript_ItemAntidote::
	finditem ITEM_ANTIDOTE
	end

PewterCave_EventScript_ItemRepel::
	finditem ITEM_REPEL
	end

PewterCave_Movement_PlayerDown::
	walk_down
	step_end

PewterCave_Movement_PlayerUp::
	walk_up
	step_end

@ (17,5) -> (13,5)
PewterCave_Movement_BrockApproach::
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

@ (13,5) -> (23,7), fuera de la pantalla, camino de la salida del este
PewterCave_Movement_BrockLeave::
	walk_fast_down
	walk_fast_down
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end
