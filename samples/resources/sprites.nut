class Sprite {
    constructor(texture, screenWidth, screenHeight) {
        this.texture = texture
        this.screenWidth  = screenWidth
        this.screenHeight = screenHeight
        // TODO: ebiten.rand
        local regionWidth  = this.screenWidth  - this.texture.width
        local regionHeight = this.screenHeight - this.texture.height
        this.x = rand() % regionWidth
        this.y = rand() % regionHeight
        this.vx = (rand() % 2) * 2 - 1
        this.vy = (rand() % 2) * 2 - 1
    }
    function update(texture) {
        // ??
        local regionWidth  = this.screenWidth  - texture.width
        local regionHeight = this.screenHeight - texture.height
        while (true) {
            this.x += this.vx
            this.y += this.vy
            if (this.x < 0) {
                this.x  = - this.x
                this.vx = -this.vx
            }
            if (regionWidth <= this.x) {
                this.x  = -this.x + 2 * regionWidth
                this.vx = -this.vx
            }
            if (this.y < 0) {
                this.y  = - this.y
                this.vy = -this.vy
            }
            if (regionHeight <= this.y) {
                this.y  = -this.y + 2 * regionHeight
                this.vy = -this.vy
            }
            ::suspend()
        }
    }
    function draw(offscreen) {
        offscreen.drawTexture(this.texture, {
                x = this.x
                y = this.y
                srcWidth = this.texture.width
                srcHeight = this.texture.height
            })
    }
    texture = null;
    screenWidth = 0
    screenHeight = 0
    x = 0
    y = 0
    vx = 1
    vy = 1
}

class Sprites {
    function update(system) {
        local input = system.input
        if (input.isTouched) {
            ::print(input.touchX + ", " + input.touchY + "\n")
        }
        if (this.screenWidth == 0 || this.screenHeight == 0) {
            return
        }
        if (sprites == null) {
            local texture = ebiten.Texture("test.png")
            this.sprites = []
            this.spritesThreads = []
            for (local i = 0; i < 100; i++) {
                local sprite = Sprite(texture, this.screenWidth, this.screenHeight)
                this.sprites.push(sprite)
                local thread = ::newthread(function () {
                        sprite.update(texture)
                    })
                this.spritesThreads.push(thread)
            }
        }
        for (local i = 0; i < this.spritesThreads.len(); i++) {
            local thread = this.spritesThreads[i]
            local status = thread.getstatus()
            if (status == "idle") {
                thread.call()
            } else if (status == "suspended") {
                thread.wakeup()
            }
        }
    }
    function draw(offscreen) {
        if (this.screenWidth == 0 || this.screenHeight == 0) {
            this.screenWidth  = offscreen.width
            this.screenHeight = offscreen.height
            return
        }
        if (this.sprites == null) {
            return
        }
        foreach (sprite in this.sprites) {
            sprite.draw(offscreen)
        }
    }
    sprites = null
    spritesThreads = null
    screenWidth  = 0
    screenHeight = 0
}

return Sprites()
