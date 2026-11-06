MtMoon_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMoon_1F_OnTransition
	.byte 0

MtMoon_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_MT_MOON_1F
	call_if_set FLAG_MT_MOON_MINER_PAID, MtMoon_1F_EventScript_MinerAside
	end

MtMoon_1F_EventScript_MinerAside::
	setobjectxyperm LOCALID_MT_MOON_MINER, 34, 37
	setobjectmovementtype LOCALID_MT_MOON_MINER, MOVEMENT_TYPE_FACE_RIGHT
	return

MtMoon_1F_EventScript_ZubatSign::
	msgbox MtMoon_1F_Text_ZubatIsABloodsucker, MSGBOX_SIGN
	end

MtMoon_1F_EventScript_Marcos::
	trainerbattle_single TRAINER_HIKER_MARCOS, MtMoon_1F_Text_MarcosIntro, MtMoon_1F_Text_MarcosDefeat
	msgbox MtMoon_1F_Text_MarcosPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Josh::
	trainerbattle_single TRAINER_YOUNGSTER_JOSH, MtMoon_1F_Text_JoshIntro, MtMoon_1F_Text_JoshDefeat
	msgbox MtMoon_1F_Text_JoshPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Miriam::
	trainerbattle_single TRAINER_LASS_MIRIAM, MtMoon_1F_Text_MiriamIntro, MtMoon_1F_Text_MiriamDefeat
	msgbox MtMoon_1F_Text_MiriamPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Iris::
	trainerbattle_single TRAINER_LASS_IRIS, MtMoon_1F_Text_IrisIntro, MtMoon_1F_Text_IrisDefeat
	msgbox MtMoon_1F_Text_IrisPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Jovan::
	trainerbattle_single TRAINER_SUPER_NERD_JOVAN, MtMoon_1F_Text_JovanIntro, MtMoon_1F_Text_JovanDefeat
	msgbox MtMoon_1F_Text_JovanPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Kent::
	trainerbattle_single TRAINER_BUG_CATCHER_KENT, MtMoon_1F_Text_KentIntro, MtMoon_1F_Text_KentDefeat
	msgbox MtMoon_1F_Text_KentPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Robby::
	trainerbattle_single TRAINER_BUG_CATCHER_ROBBY, MtMoon_1F_Text_RobbyIntro, MtMoon_1F_Text_RobbyDefeat
	msgbox MtMoon_1F_Text_RobbyPostBattle, MSGBOX_AUTOCLOSE
	end

@ Soldado: el camino a CIUDAD CELESTE está cortado por un derrumbe
MtMoon_1F_EventScript_Soldier::
	msgbox MtMoon_1F_Text_SoldierCollapse, MSGBOX_NPC
	end

@ Minero del atajo (35,37): cobra 15000 una sola vez y se aparta a (34,37)
@ para dejar libre la puerta a MT_MOON_PARTE_1
MtMoon_1F_EventScript_Miner::
	lock
	faceplayer
	msgbox MtMoon_1F_Text_MinerMotto
	goto_if_set FLAG_MT_MOON_MINER_PAID, MtMoon_1F_EventScript_MinerAlreadyPaid
	specialvar VAR_RESULT, FindPartyMeowth
	goto_if_ne VAR_RESULT, PARTY_SIZE, MtMoon_1F_EventScript_MinerMeowth
	goto MtMoon_1F_EventScript_MinerAskMoney
	end

@ Su POKÉMON favorito: si llevas un MEOWTH te deja pasar a cambio de él
MtMoon_1F_EventScript_MinerMeowth::
	msgbox MtMoon_1F_Text_MinerMeowthExcited
	specialvar VAR_RESULT, CountPartyNonEggMons
	goto_if_eq VAR_RESULT, 1, MtMoon_1F_EventScript_MinerMeowthOnlyMon
	msgbox MtMoon_1F_Text_MinerAskMeowth, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtMoon_1F_EventScript_MinerMeowthDeclined
	special GiveAwayPartyMonAt
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_TMHM
	message MtMoon_1F_Text_GaveMeowth
	waitmessage
	waitfanfare
	textcolor NPC_TEXT_COLOR_MALE
	msgbox MtMoon_1F_Text_MinerThanksMeowth
	closemessage
	goto MtMoon_1F_EventScript_MinerStepAside
	end

MtMoon_1F_EventScript_MinerMeowthOnlyMon::
	msgbox MtMoon_1F_Text_MinerMeowthOnlyMon
	goto MtMoon_1F_EventScript_MinerAskMoney
	end

MtMoon_1F_EventScript_MinerMeowthDeclined::
	msgbox MtMoon_1F_Text_MinerMeowthDeclined
	goto MtMoon_1F_EventScript_MinerAskMoney
	end

MtMoon_1F_EventScript_MinerAskMoney::
	showmoneybox 0, 0
	msgbox MtMoon_1F_Text_MinerOffer
	msgbox MtMoon_1F_Text_MinerAskPay, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtMoon_1F_EventScript_MinerDeclined
	checkmoney 15000
	goto_if_eq VAR_RESULT, FALSE, MtMoon_1F_EventScript_MinerNoMoney
	removemoney 15000
	updatemoneybox
	playse SE_SHOP
	msgbox MtMoon_1F_Text_MinerDeal
	waitse
	hidemoneybox
	closemessage
	goto MtMoon_1F_EventScript_MinerStepAside
	end

MtMoon_1F_EventScript_MinerStepAside::
	call_if_eq VAR_FACING, DIR_EAST, MtMoon_1F_EventScript_MinerPlayerStepUp
	applymovement LOCALID_MT_MOON_MINER, MtMoon_1F_Movement_MinerStepAside
	waitmovement 0
	setobjectxyperm LOCALID_MT_MOON_MINER, 34, 37
	setobjectmovementtype LOCALID_MT_MOON_MINER, MOVEMENT_TYPE_FACE_RIGHT
	setflag FLAG_MT_MOON_MINER_PAID
	release
	end

MtMoon_1F_EventScript_MinerPlayerStepUp::
	applymovement LOCALID_PLAYER, Common_Movement_WalkUp
	waitmovement 0
	return

MtMoon_1F_EventScript_MinerAlreadyPaid::
	msgbox MtMoon_1F_Text_MinerAlreadyPaid
	release
	end

MtMoon_1F_EventScript_MinerDeclined::
	msgbox MtMoon_1F_Text_MinerDeclined
	hidemoneybox
	release
	end

MtMoon_1F_EventScript_MinerNoMoney::
	msgbox MtMoon_1F_Text_MinerNoMoney
	hidemoneybox
	release
	end

MtMoon_1F_Movement_MinerStepAside::
	walk_left
	face_right
	step_end
