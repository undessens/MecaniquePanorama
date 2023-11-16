import keyboard
import os
import winsound

played = False
print('Sonnette prete')
while True:
	if (keyboard.is_pressed("1") or
		keyboard.is_pressed("2") or
		keyboard.is_pressed("3") or
		keyboard.is_pressed("4") or
		keyboard.is_pressed("5")):

		if not played:
			print("Ding Dong")
			audio_file = os.path.dirname(__file__)  + '/Sounds/Cloche1.wav'
			winsound.PlaySound(audio_file, winsound.SND_ASYNC | winsound.SND_ALIAS)
			played = True
	else:
		played = False