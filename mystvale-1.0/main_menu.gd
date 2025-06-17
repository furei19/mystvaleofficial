extends Control

func _on_start_pressed() -> void:
	get_tree().change_scene_to_file("res://scenes/select_race.tscn")

func _on_load_pressed() -> void:
	pass # Replace with function body.

func _on_exit_pressed() -> void:
	get_tree().quit()
