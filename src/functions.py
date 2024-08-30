from src.settings import *

class DrawImage():
    def __init__(img_path, width, height):
        set_draw_image = customtkinter.CTkImage(
            dark_image = Image.open(img_path),
            size = (width, height)
        )
        return set_draw_image