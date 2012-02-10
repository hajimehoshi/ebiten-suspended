// To instinate
g_texture <- null

// draw の廃止
// update の中で何か?

function test_update_Texture_constructor(system) {
    {
        local t = ebiten.Texture(10, 20)
        ::assert(t.width  == 10)
        ::assert(t.height == 20)
    }
    {
        // TODO: path?
        // TODO: Use system.loadTexture
        local t = ebiten.Texture("samples/resources/test.png")
        ::assert(t.width  == 32)
        ::assert(t.height == 32)
    }
    {
        ::g_texture = ebiten.Texture("samples/resources/blocks.png")
        ::assert(g_texture.width  == 70)
        ::assert(g_texture.height == 10)
    }
}

/*// temporal
t <- null

function test_draw_Texture_drawTexture(offscreen) {
    ::assert(offscreen != null)
    ::t = ebiten.Texture("samples/resources/blocks.png")
    offscreen.drawTexture(::t, {})
}*/
