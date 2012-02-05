function test_update_Texture_constructor(system) {
    local t = ebiten.Texture(10, 20)
    ::assert(t.width == 10)
    ::assert(t.height == 20)
}
