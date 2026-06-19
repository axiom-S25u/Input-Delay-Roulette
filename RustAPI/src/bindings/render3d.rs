use crate::ffi;

pub struct Camera;

impl Camera {
    pub fn set(x: f32, y: f32, z: f32, rx: f32, ry: f32, rz: f32) {
        unsafe { ffi::geode_render3d_set_camera(x, y, z, rx, ry, rz) }
    }
}

// Basic 3D types
pub struct Vec3 {
    pub x: f32,
    pub y: f32,
    pub z: f32,
}

pub struct Rot3 {
    pub x: f32,
    pub y: f32,
    pub z: f32,
}
