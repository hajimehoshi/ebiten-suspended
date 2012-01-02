class Game {
    constructor(width, heigth) {
        this.texture = null
        this.width = width
        this.height = height
    }
    function update() {
        if (!this.texture) {
            // TODO: resource path
            //this.texture = ebiten.Texture("test.png")
        }
        //::print(this.texture.isCreated() + " ")
        //::print(this.texture.getWidth() + " " + this.texture.getHeight() + "\n")
        /*if (this.offscreen) {
            ::print(this.offscreen.getWidth() + "\n")
        }*/
        this.x++
    }
    function draw(offscreen) {
        //::print("draw!\n");
        //::print(offscreen.getWidth() + "\n");
        offscreen.drawRect(this.x, 10, 100, 100, 255, 0, 0, 255)
        // WTF!?
        //this.offscreen = offscreen
        ::print(offscreen + "\n")
    }
    offscreen = null
    texture = null
    x = 0
    width = 0
    height = 0
}

return Game(320, 240)
