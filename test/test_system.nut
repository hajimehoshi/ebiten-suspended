function test_update_system(system) {
    ::assert(system instanceof ebiten.System);
    local input = system.input
    ::assert(input instanceof ebiten.Input);
    ::assert(typeof(input.touchX) == "integer");
    ::assert(typeof(input.touchY) == "integer");
    ::assert(typeof(input.isTouched) == "bool");
}
