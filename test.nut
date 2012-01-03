class Game {
    constructor(width, heigth) {
        this.texture = null
        this.width = width
        this.height = height
    }
    function update() {
        if (!this.texture) {
            // TODO: resource path
            this.texture = ebiten.Texture("/Users/hajime/ebiten/test.png")
        }
        if (!this.sprite) {
            this.sprite = ebiten.Sprite(this.texture)
            this.sprite.setSrcWidth(32)
            this.sprite.setSrcHeight(32)
        }
        this.sprite.setX(this.x);
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

class Foo {
    function _set(name, value) {
        ::print(name + "\n")
    }
}

foo <- Foo()
foo.bar = "aa"

return Game(320, 240)
