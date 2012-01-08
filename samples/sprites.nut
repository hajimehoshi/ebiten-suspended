class Sprite {
    constructor(texture, screenWidth, screenHeight) {
        this.screenWidth  = screenWidth
        this.screenHeight = screenHeight
        // TODO: path
        this.eSprite = ebiten.Sprite(texture)
        // Texture is not created then!
        this.eSprite.srcWidth  = 32
        this.eSprite.srcHeight = 32
        // TODO: ebiten.rand
        local regionWidth  = this.screenWidth  - this.eSprite.srcWidth
        local regionHeight = this.screenHeight - this.eSprite.srcHeight
        this.eSprite.x = rand() % regionWidth
        this.eSprite.y = rand() % regionHeight
        this.vx = (rand() % 2) * 2 - 1
        this.vy = (rand() % 2) * 2 - 1
    }
    function update() {
        this.eSprite.x += this.vx
        this.eSprite.y += this.vy
        local regionWidth  = this.screenWidth  - this.eSprite.srcWidth
        local regionHeight = this.screenHeight - this.eSprite.srcHeight
        if (this.eSprite.x < 0) {
            this.eSprite.x = - this.eSprite.x
            this.vx        = -this.vx
        }
        if (regionWidth <= this.eSprite.x) {
            this.eSprite.x = -this.eSprite.x + 2 * regionWidth
            this.vx        = -this.vx
        }
        if (this.eSprite.y < 0) {
            this.eSprite.y = - this.eSprite.y
            this.vy        = -this.vy
        }
        if (regionHeight <= this.eSprite.y) {
            this.eSprite.y = -this.eSprite.y + 2 * regionHeight
            this.vy        = -this.vy
        }
    }
    function draw(offscreen) {
        offscreen.drawSprite(this.eSprite)
    }
    eSprite = null
    screenWidth = 0
    screenHeight = 0
    vx = 1
    vy = 1
}

class Sprites {
    function update() {
        if (this.screenWidth == 0 || this.screenHeight == 0) {
            return
        }
        if (sprites == null) {
            local texture = ebiten.Texture("/Users/hajime/ebiten/test.png")
            this.sprites = []
            for (local i = 0; i < 1000; i++) {
                this.sprites.push(Sprite(texture, this.screenWidth, this.screenHeight))
            }
        }
        foreach (sprite in this.sprites) {
            sprite.update()
        }
    }
    function draw(offscreen) {
        if (this.screenWidth == 0 || this.screenHeight == 0) {
            this.screenWidth  = offscreen.width
            this.screenHeight = offscreen.height
            return
        }
        foreach (sprite in this.sprites) {
            sprite.draw(offscreen)
        }
    }
    sprites = null
    screenWidth  = 0
    screenHeight = 0
}

return Sprites()
