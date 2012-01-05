class Game {
    constructor(width, heigth) {
        this.texture = null
        this.width = width
        this.height = height
    }
    function update() {
        ebiten.terminate()
        if (!this.texture) {
            // TODO: resource path
            this.texture = ebiten.Texture("/Users/hajime/ebiten/test.png")
        }
        if (!this.sprite) {
            this.sprite = ebiten.Sprite(this.texture)
            this.sprite.srcWidth = 32
            this.sprite.srcHeight = 32
        }
        /*local a = []
        a.remove(true)*/
        this.sprite.x = this.x;
        this.sprite.alpha = 0.8;
        this.x++
    }
    function draw(offscreen) {
        offscreen.drawRect(this.x, 10, 100, 100, 255, 0, 0, 255)
        offscreen.drawSprite(this.sprite)
    }
    texture = null
    sprite = null
    x = 0
    width = 0
    height = 0
}

return Game(320, 240)
