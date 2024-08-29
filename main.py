import customtkinter
from PIL import Image

app_width = 1280
app_height = 720
app_window = f'{app_width}x{app_height}'
bg_img_path = 'pygtavmenu\\img\\background.png'


# Resource for drawing images.
def draw_image(path, width, height):
    set_image = customtkinter.CTkImage(
        dark_image = Image.open(path),
        size = (width, height)
    )
    return set_image

class draw_rect(customtkinter.CTkFrame):
    def __init__(self, master):
        width = 100
        height = 20
        fg_color = '#0051ff'
        MenuBGImage = 1
        super().__init__(master, width = width, height = height, fg_color = fg_color, corner_radius = 0)
        # if MenuBGImage == '1':
            # self.image_path = bg_custom_img_path
            # self.GetBGImage = DrawImage(self.image_path, width = width, height = height)
            # self.DrawBGImage = customtkinter.CTkLabel(self, image = self.GetBGImage, text = "")
            # self.DrawBGImage.grid(row = 0, column = 0)

class App(customtkinter.CTk):
    def __init__(self):
        super().__init__()
        self.title("GTAV Ps3 Menu Designer")
        self.geometry(app_window)
        self.resizable(False, False)

        # Imagem de fundo do app.
        self.get_bg_image = draw_image(bg_img_path, app_width, app_height)
        self.app_bg_image = customtkinter.CTkLabel(self, image = self.get_bg_image, text = "")
        self.app_bg_image.grid(row = 0, column = 0)

        self.DrawBackground = draw_rect(self)
        self.DrawBackground.place(x = 10, y = 10)

if __name__ == '__main__':
    app = App()
    app.mainloop()