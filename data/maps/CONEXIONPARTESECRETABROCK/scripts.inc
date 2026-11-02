CONEXIONPARTESECRETABROCK_MapScripts::
	.byte 0

@ Salida inferior (7,9): sube al pasadizo secreto del gimnasio, detrás de la pared del fondo
SecretGymLink_EventScript_ToGym::
	lockall
	playse SE_EXIT
	warp MAP_PEWTER_CITY_GYM, 9, 1
	waitstate
	releaseall
	end
