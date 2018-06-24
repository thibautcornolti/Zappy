import AssetsPool from "../AssetsPool";
import { Object3D, Vector2, Vector3, Clock } from "three";
import GUIManagger from "../GUIManager";
import AudioManager from "../AudioManager";
// import * as THREE from "three";
// import "../../types/gpu-particle-system.d.ts";
import "gpu-particle";

/// <reference path="../../types/gpu-particle-system.d.ts" />


let options = {
    position: new Vector3(),
    positionRandomness: 3,
    velocity: new Vector3(),
    velocityRandomness: 3,
    color: 0xaa88ff,
    colorRandomness: .2,
    turbulence: .5,
    lifetime: 0.5,
    size: 5,
    sizeRandomness: 1
};

export default class Player {
    private object: Object3D;
    private dest: Vector3;
    private speedX: number;
    private speedZ: number;
    private destRot: Vector3;
    private speedRot: number;
    private timeInterval: any;
    private timeIntervalRot: any;

    private particleInterval: any;
    private clock: Clock;
    private particle: THREE.GPUParticleSystem;
    private inc: number;

    constructor(assetPool: AssetsPool, position: Vector2 = new Vector2(0, 0)) {
        if (!assetPool.getGltfAssets("chicken")) {
            alert("Missing models: Chicken");
            window.location.href = "/";
        }

        this.timeInterval = null;
        this.timeIntervalRot = null;
        this.speedX = 0;
        this.speedZ = 0;
        this.speedRot = 0;
        this.object = assetPool.getGltfAssets("chicken").scene.clone();
        this.dest = new Vector3(position.x, 0, position.y);
        this.destRot = new Vector3(position.x, 0, position.y);
        this.object.position.set(position.x, 0, position.y);
        GUIManagger.getInstance().getScene().add(this.object);
        this.particle = new THREE.GPUParticleSystem({
            maxParticles: 250000
        });
        this.particleInterval = undefined;
        this.inc = 0;
        this.clock = new Clock();
        GUIManagger.getInstance().getScene().add((this.particle as any));
        // this.setParticle(true);
    }

    public setPosition(pos: Vector3) {
        let count = 5;
        if (this.timeInterval) {
            clearInterval(this.timeInterval);
            this.timeInterval = null;
        }
        this.dest = pos;
        this.speedX = Math.abs(pos.x - this.object.position.x) / count;
        this.speedZ = Math.abs(pos.z - this.object.position.z) / count;
        this.timeInterval = setInterval(() => {
            count--;
            if (!count && this.timeInterval) {
                clearInterval(this.timeInterval);
                this.timeInterval = null;
            }
            let newX = this.object.position.x;
            let newZ = this.object.position.z;
            if (this.dest.x > this.object.position.x)
                newX += this.speedX;
            else if (this.dest.x < this.object.position.x)
                newX -= this.speedX;
            if (this.dest.z > this.object.position.z)
                newZ += this.speedZ;
            else if (this.dest.z < this.object.position.z)
                newZ -= this.speedZ;
            this.object.position.set(newX, this.dest.y, newZ);
        }, 25);
    }

    public setRotation(rotation: Vector3) {
        let count = 5;
        if (this.timeIntervalRot) {
            clearInterval(this.timeIntervalRot);
            this.timeIntervalRot = null;
        }
        this.destRot = rotation
        this.speedRot = Math.abs(rotation.y - this.object.rotation.y) / count;
        this.timeIntervalRot = setInterval(() => {
            count--;
            if (!count && this.timeIntervalRot) {
                clearInterval(this.timeIntervalRot);
                this.timeIntervalRot = null;
            }
            let newRot = this.object.rotation.y;
            if (this.destRot.y > this.object.rotation.y)
                newRot += this.speedRot;
            else if (this.destRot.y < this.object.rotation.y)
                newRot -= this.speedRot;
            this.object.rotation.set(this.destRot.x, newRot, this.destRot.z);
        }, 25, 0);
    }

    public remove() {
        GUIManagger.getInstance().getScene().remove(this.object);
        let audio = AudioManager.getInstance().getSound("chickenDeath");
        if (this.particleInterval)
            clearInterval(this.particleInterval);
        if (audio)
            audio.play();
    }

    public setParticle(state: true) {
        this.particleInterval = setInterval(() => {
            let delta = this.clock.getDelta();

            this.inc += delta;
            options.position.x = this.object.position.x;
            options.position.y = this.object.position.y;
            options.position.z = this.object.position.z;
            for (let i = 0; i < 15000; i++) {
                this.particle.spawnParticle(options);
            }
            this.particle.update(this.inc);
            console.log(this.clock.getDelta());
        }, 10);
    }
}