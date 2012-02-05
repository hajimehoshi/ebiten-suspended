function test_update_GeometryMatrix_constructor(system) {
    local g = ebiten.GeometryMatrix(3.1, 4.1, 5.9, 2.6, 5.3, 5.8)
    ::assert(g instanceof ebiten.GeometryMatrix)
    ::assert(g.a == 3.1)
    ::assert(g.b == 4.1)
    ::assert(g.c == 5.9)
    ::assert(g.d == 2.6)
    ::assert(g.tx == 5.3)
    ::assert(g.ty == 5.8)
}

function test_update_GeometryMatrix_identity(system) {
    local g = ebiten.GeometryMatrix.identity
    ::assert(g instanceof ebiten.GeometryMatrix)
    ::assert(g.a == 1)
    ::assert(g.b == 0)
    ::assert(g.c == 0)
    ::assert(g.d == 1)
    ::assert(g.tx == 0)
    ::assert(g.ty == 0)
}

function test_update_GeometryMatrix_concat(system) {
    local a = ebiten.GeometryMatrix(3.1, 4.1, 5.9, 2.6, 5.3, 5.8)
    local b = ebiten.GeometryMatrix(9.7, 9.3, 2.3, 8.4, 6.2, 6.4)
    {
        local c = a.concat(b)
        ::assertDelta(a.a,  3.1,  0.0001)
        ::assertDelta(a.b,  4.1,    0.0001)
        ::assertDelta(a.c,  5.9,    0.0001)
        ::assertDelta(a.d,  2.6,    0.0001)
        ::assertDelta(a.tx, 5.3,    0.0001)
        ::assertDelta(a.ty, 5.8,    0.0001)
        ::assertDelta(b.a,  9.7,    0.0001)
        ::assertDelta(b.b,  9.3,    0.0001)
        ::assertDelta(b.c,  2.3,    0.0001)
        ::assertDelta(b.d,  8.4,    0.0001)
        ::assertDelta(b.tx, 6.2,    0.0001)
        ::assertDelta(b.ty, 6.4,    0.0001)
        ::assertDelta(c.a,  84.94,  0.0001)
        ::assertDelta(c.b,  63.95,  0.0001)
        ::assertDelta(c.c,  56.69,  0.0001)
        ::assertDelta(c.d,  31.27,  0.0001)
        ::assertDelta(c.tx, 111.55, 0.0001)
        ::assertDelta(c.ty, 67.31,  0.0001)
    }
    {
        local c = b.concat(a)
        ::assertDelta(a.a,  3.1,   0.0001)
        ::assertDelta(a.b,  4.1,   0.0001)
        ::assertDelta(a.c,  5.9,   0.0001)
        ::assertDelta(a.d,  2.6,   0.0001)
        ::assertDelta(a.tx, 5.3,   0.0001)
        ::assertDelta(a.ty, 5.8,   0.0001)
        ::assertDelta(b.a,  9.7,   0.0001)
        ::assertDelta(b.b,  9.3,   0.0001)
        ::assertDelta(b.c,  2.3,   0.0001)
        ::assertDelta(b.d,  8.4,   0.0001)
        ::assertDelta(b.tx, 6.2,   0.0001)
        ::assertDelta(b.ty, 6.4,   0.0001)
        ::assertDelta(c.a,  39.5,  0.0001)
        ::assertDelta(c.b,  63.27, 0.0001)
        ::assertDelta(c.c,  63.21, 0.0001)
        ::assertDelta(c.d,  76.71, 0.0001)
        ::assertDelta(c.tx, 50.76, 0.0001)
        ::assertDelta(c.ty, 59.02, 0.0001)
    }
    {
        ::assertError(function () {
                a.concat(Foo())                
            })
    }
}

function test_update_GeometryMatrix_scale(system) {
    local g = ebiten.GeometryMatrix.scale(3.1, 4.1)
    ::assert(g instanceof ebiten.GeometryMatrix)
    ::assert(g.a == 3.1)
    ::assert(g.b == 0)
    ::assert(g.c == 0)
    ::assert(g.d == 4.1)
    ::assert(g.tx == 0)
    ::assert(g.ty == 0)
}

function test_update_GeometryMatrix_scaleWithCenter(system) {
}

function test_update_GeometryMatrix_rotate(system) {
    local theta = 0
    {
        local g = ebiten.GeometryMatrix.rotate(theta)
        ::assert(g instanceof ebiten.GeometryMatrix)
        ::assertDelta(g.a, ::cos(theta),  0.0001)
        ::assertDelta(g.b, -::sin(theta), 0.0001)
        ::assertDelta(g.c, ::sin(theta),  0.0001)
        ::assertDelta(g.d, ::cos(theta),  0.0001)
        ::assert(g.tx == 0)
        ::assert(g.ty == 0)
    }
    local theta = 1
    {
        local g = ebiten.GeometryMatrix.rotate(theta)
        ::assert(g instanceof ebiten.GeometryMatrix)
        ::assertDelta(g.a, ::cos(theta),  0.0001)
        ::assertDelta(g.b, -::sin(theta), 0.0001)
        ::assertDelta(g.c, ::sin(theta),  0.0001)
        ::assertDelta(g.d, ::cos(theta),  0.0001)
        ::assert(g.tx == 0)
        ::assert(g.ty == 0)
    }
}

function test_update_GeometryMatrix_rotateWithCenter(system) {
}
