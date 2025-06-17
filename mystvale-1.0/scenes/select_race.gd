extends Control


func _on_human_pressed() -> void:
	$VBoxContainer/Human.set_pressed(true)
	$VBoxContainer/Elf.set_pressed(false)
	$VBoxContainer/Dwarf.set_pressed(false)
	$VBoxContainer/Giant.set_pressed(false)

	$defaultlabel.hide()
	$humanlabel.show()
	$elflabel.hide()
	$dwarflabel.hide()
	$giantlabel.hide()

func _on_elf_pressed() -> void:
	$VBoxContainer/Human.set_pressed(false)
	$VBoxContainer/Elf.set_pressed(true)
	$VBoxContainer/Dwarf.set_pressed(false)
	$VBoxContainer/Giant.set_pressed(false)

	$defaultlabel.hide()
	$humanlabel.hide()
	$elflabel.show()
	$dwarflabel.hide()
	$giantlabel.hide()

func _on_dwarf_pressed() -> void:
	$VBoxContainer/Human.set_pressed(false)
	$VBoxContainer/Elf.set_pressed(false)
	$VBoxContainer/Dwarf.set_pressed(true)
	$VBoxContainer/Giant.set_pressed(false)
	
	$defaultlabel.hide()
	$humanlabel.hide()
	$elflabel.hide()
	$dwarflabel.show()
	$giantlabel.hide()

func _on_giant_pressed() -> void:
	$VBoxContainer/Human.set_pressed(false)
	$VBoxContainer/Elf.set_pressed(false)
	$VBoxContainer/Dwarf.set_pressed(false)
	$VBoxContainer/Giant.set_pressed(true)

	$defaultlabel.hide()
	$humanlabel.hide()
	$elflabel.hide()
	$dwarflabel.hide()
	$giantlabel.show()
	
