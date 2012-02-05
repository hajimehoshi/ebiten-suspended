function test_update_ColorMatrix_constructor(system) {
    {
        local elements = [
            3.1, 4.1, 5.9, 2.6, 5.3,
            5.8, 9.7, 9.3, 2.3, 8.4,
            6.2, 6.4, 3.3, 8.3, 2.7,
            9.5, 0.2, 8.8, 4.1, 9.7,
        ]
        local c = ebiten.ColorMatrix(elements)
        ::assert(c instanceof ebiten.ColorMatrix)
        ::assert(c.e00 == 3.1)
        ::assert(c.e01 == 4.1)
        ::assert(c.e02 == 5.9)
        ::assert(c.e03 == 2.6)
        ::assert(c.e04 == 5.3)
        ::assert(c.e10 == 5.8)
        ::assert(c.e11 == 9.7)
        ::assert(c.e12 == 9.3)
        ::assert(c.e13 == 2.3)
        ::assert(c.e14 == 8.4)
        ::assert(c.e20 == 6.2)
        ::assert(c.e21 == 6.4)
        ::assert(c.e22 == 3.3)
        ::assert(c.e23 == 8.3)
        ::assert(c.e24 == 2.7)
        ::assert(c.e30 == 9.5)
        ::assert(c.e31 == 0.2)
        ::assert(c.e32 == 8.8)
        ::assert(c.e33 == 4.1)
        ::assert(c.e34 == 9.7)
        ::assert(c.e40 == 0)
        ::assert(c.e41 == 0)
        ::assert(c.e42 == 0)
        ::assert(c.e43 == 0)
        ::assert(c.e44 == 1)
        ::assertError(function () {
                local v = c.e54
            })
        ::assertError(function () {
                local v = c.e45
            })
        ::assertError(function () {
                local v = c.eaa
            })
    }
    {
        local elements = [
            3.1, 4.1, 5.9, 2.6, 5.3,
            5.8, 9.7, 9.3, 2.3, 8.4,
            6.2, 6.4, 3.3, 8.3, 2.7,
            9.5, 0.2, 8.8, 4.1,
        ]
        ::assertError(function () {
                local c = ebiten.ColorMatrix(elements)
            })
    }
    {
        local elements = [
            3.1, 4.1, 5.9, 2.6, 5.3,
            5.8, 9.7, 9.3, 2.3, 8.4,
            6.2, 6.4, 3.3, 8.3, 2.7,
            9.5, 0.2, 8.8, 4.1, 9.7,
            1.6,
        ]
        ::assertError(function () {
                local c = ebiten.ColorMatrix(elements)
            })
    }
}

function test_update_ColorMatrix_identity(system) {
    local c = ebiten.ColorMatrix.identity
    ::assert(c instanceof ebiten.ColorMatrix)
    ::assert(c.e00 == 1)
    ::assert(c.e01 == 0)
    ::assert(c.e02 == 0)
    ::assert(c.e03 == 0)
    ::assert(c.e04 == 0)
    ::assert(c.e10 == 0)
    ::assert(c.e11 == 1)
    ::assert(c.e12 == 0)
    ::assert(c.e13 == 0)
    ::assert(c.e14 == 0)
    ::assert(c.e20 == 0)
    ::assert(c.e21 == 0)
    ::assert(c.e22 == 1)
    ::assert(c.e23 == 0)
    ::assert(c.e24 == 0)
    ::assert(c.e30 == 0)
    ::assert(c.e31 == 0)
    ::assert(c.e32 == 0)
    ::assert(c.e33 == 1)
    ::assert(c.e34 == 0)
    ::assert(c.e40 == 0)
    ::assert(c.e41 == 0)
    ::assert(c.e42 == 0)
    ::assert(c.e43 == 0)
    ::assert(c.e44 == 1)
}

function test_update_ColorMatrix_concat(system) {
    local c1 = ebiten.ColorMatrix([
            3.1, 4.1, 5.9, 2.6, 5.3,
            5.8, 9.7, 9.3, 2.3, 8.4,
            6.2, 6.4, 3.3, 8.3, 2.7,
            9.5, 0.2, 8.8, 4.1, 9.7,
        ])
    local c2 = ebiten.ColorMatrix([
            1.6, 9.3, 9.9, 3.8, 1.9,
            6.8, 2.1, 4.8, 0.8, 6.5,
            1.3, 2.8, 2.3, 0.6, 6.4,
            7.0, 9.3, 8.4, 4.6, 0.9,
        ])
    {
        local c3 = c1.concat(c2)
        ::assertDelta(c3.e00, 156.38, 0.0001)
        ::assertDelta(c3.e01, 160.89, 0.0001)
        ::assertDelta(c3.e02, 162.04, 0.0001)
        ::assertDelta(c3.e03, 123.30, 0.0001)
        ::assertDelta(c3.e04, 152.09, 0.0001)
        ::assertDelta(c3.e10, 70.62,  0.0001)
        ::assertDelta(c3.e11, 79.13,  0.0001)
        ::assertDelta(c3.e12, 82.53,  0.0001)
        ::assertDelta(c3.e13, 65.63,  0.0001)
        ::assertDelta(c3.e14, 80.90,  0.0001)
        ::assertDelta(c3.e20, 40.23,  0.0001)
        ::assertDelta(c3.e21, 47.33,  0.0001)
        ::assertDelta(c3.e22, 46.58,  0.0001)
        ::assertDelta(c3.e23, 31.37,  0.0001)
        ::assertDelta(c3.e24, 48.84,  0.0001)
        ::assertDelta(c3.e30, 171.42, 0.0001)
        ::assertDelta(c3.e31, 173.59, 0.0001)
        ::assertDelta(c3.e32, 195.99, 0.0001)
        ::assertDelta(c3.e33, 128.17, 0.0001)
        ::assertDelta(c3.e34, 183.42, 0.0001)
        ::assert(c3.e40 == 0)
        ::assert(c3.e41 == 0)
        ::assert(c3.e42 == 0)
        ::assert(c3.e43 == 0)
        ::assert(c3.e44 == 1)
    }
    {
        local c3 = c2.concat(c1)
        ::assertDelta(c3.e00, 58.71,  0.0001)
        ::assertDelta(c3.e01, 78.14,  0.0001)
        ::assertDelta(c3.e02, 85.78,  0.0001)
        ::assertDelta(c3.e03, 30.56,  0.0001)
        ::assertDelta(c3.e04, 77.94,  0.0001)
        ::assertDelta(c3.e10, 103.43, 0.0001)
        ::assertDelta(c3.e11, 121.74, 0.0001)
        ::assertDelta(c3.e12, 144.69, 0.0001)
        ::assertDelta(c3.e13, 45.96,  0.0001)
        ::assertDelta(c3.e14, 144.06, 0.0001)
        ::assertDelta(c3.e20, 115.83, 0.0001)
        ::assertDelta(c3.e21, 157.53, 0.0001)
        ::assertDelta(c3.e22, 169.41, 0.0001)
        ::assertDelta(c3.e23, 68.84,  0.0001)
        ::assertDelta(c3.e24, 84.67,  0.0001)
        ::assertDelta(c3.e30, 56.70,  0.0001)
        ::assertDelta(c3.e31, 151.54, 0.0001)
        ::assertDelta(c3.e32, 149.69, 0.0001)
        ::assertDelta(c3.e33, 60.40,  0.0001)
        ::assertDelta(c3.e34, 89.06,  0.0001)
        ::assert(c3.e40 == 0)
        ::assert(c3.e41 == 0)
        ::assert(c3.e42 == 0)
        ::assert(c3.e43 == 0)
        ::assert(c3.e44 == 1)
    }
    ::assertError(function () {
            c1.concat(Foo())                
        })
}
