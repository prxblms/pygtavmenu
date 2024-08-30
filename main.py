from src.functions import *

class App(customtkinter.CTk):
    def __init__(self):
        super().__init__()

        self.title(" GTA V Ps3 Menu Designer ")
        self.geometry(app_window)
        self.resizable(False, False)

        self.app_bg_image(self)

    def app_bg_image(self):
        img_path = '.\\img\\background.png'
        width = app_width
        height = app_height
        self.app_background = DrawImage(img_path, width, height)
        self.app_bg_image = customtkinter.CTkLabel(self, image = self.app_background, text = "")
        self.app_bg_image.grid(row = 0, column = 0)

if __name__ == '__main__':
    app = App().mainloop()