import AssetsPool from "../AssetsPool";
import GUIManager from "../GUIManager";
import {Object3D, Vector2, Vector3, Clock, Audio, Mesh, SphereGeometry, MeshBasicMaterial, CubeGeometry, CubeTexture, Material} from "three";
import AudioManager from "../AudioManager";
// import * as THREE from "three";
// import "../../types/gpu-particle-system.d.ts";
import "gpu-particle";

/// <reference path="../../types/gpu-particle-system.d.ts" />

let options = {
    position: new Vector3(),
    positionRandomness: 2,
    velocity: new Vector3(),
    velocityRandomness: 2,
    color: 0xaa88ff,
    colorRandomness: .2,
    turbulence: 0,
    lifetime: 0.5,
    size: 2,
    sizeRandomness: 0.5
};

let teams: {[index:string]: number} = {};
let idTeam = 0;

export default class Player {
    private removed: boolean;
    private object: Object3D;
    private tempObjects: Object3D[];
    private tempObjectsTimeout: any;
    private teamMarker: Object3D | undefined;
    private broadcastBubbleTexture: Mesh;
    private broadcastBubbleInterval: any;
    private broadcastBubbleTimeout: any;
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
    private particleEnable: boolean;
    private incantationSound: Audio | undefined;

    constructor(assetPool: AssetsPool, teamName: string, level: number, position: Vector2 = new Vector2(0, 0)) {
        if (!assetPool.getGltfAssets("chicken")) {
            alert("Missing models: Chicken");
            window.location.href = "/";
        }

        this.removed = false;
        this.teamMarker = undefined;
        this.timeInterval = null;
        this.timeIntervalRot = null;
        this.speedX = 0;
        this.speedZ = 0;
        this.speedRot = 0;
        this.object = assetPool.getGltfAssets("chicken").scene.clone();
        this.tempObjects = [
            assetPool.getGltfAssets("chicken_inventory").scene.clone(),
            assetPool.getGltfAssets("chicken_looking").scene.clone(),
        ]
        this.broadcastBubbleTexture = assetPool.getPlaneMesh("bubble").clone();
        this.setLevel(level);
        this.dest = new Vector3(position.x, 0, position.y);
        this.destRot = new Vector3(position.x, 0, position.y);
        this.object.position.set(position.x, 0, position.y);
        this.initTeamMarker(teamName);
        GUIManager.getInstance().getScene().add(this.object);

        this.incantationSound = undefined;
        this.setIncantationSound();


        // HERE IS HOW ANIMATE A JSON ASSET

        // let object = assetPool.getJsonAssets("test").clone();
        // object.position.set(position.x, 0, position.y);
        // object.scale.set(0.1, 0.1, 0.1)
        // let mixer = new THREE.AnimationMixer(object);
        // if (object.geometry instanceof THREE.Geometry)
        //     mixer.clipAction(object.geometry.animations[0]).setDuration(1).play();
        // GUIManager.getInstance().getMixers().push(mixer);
        // GUIManager.getInstance().getMixers().pop();
        // GUIManager.getInstance().getScene().add(object);
        // GUIManager.getInstance().getScene().remove(object);
        // object.remove();

        this.particle = new THREE.GPUParticleSystem({
            maxParticles: 250000
        });
        this.particleEnable = false;
        this.inc = 0;
        this.clock = new Clock();
        GUIManager.getInstance().getScene().add((this.particle as any));
        this.particleInterval = setInterval(this.emitParticle, 25);
    }

    private getColorFromTeamName(teamName: string): number {
        if (teams[teamName])
            return teams[teamName];
        let r = Math.round(Math.random() * 127 + 127);
        let g = Math.round(Math.random() * 127 + 127);
        let b = Math.round(Math.random() * 127 + 127);
        let color = (r << 16) | (g << 8) | b;
        teams[teamName] = color;
        return color;
    }

    private initTeamMarker(teamName: string) {
        let geometry = new SphereGeometry(0.3, 10, 10);
        let material = new MeshBasicMaterial({color: this.getColorFromTeamName(teamName)});
        this.teamMarker = new Mesh(geometry, material);
        this.teamMarker.position.y = 4;
        this.object.add(this.teamMarker);
    }

    private setIncantationSound() {
        let audio = AudioManager.getInstance().getSound("incantationStart");

        if (audio)
            this.incantationSound = audio;
    }

    private emitParticle = () => {
        this.inc += this.clock.getDelta();
        if (this.particleEnable) {
            options.position = this.object.position;
            for (let i = 0; i < 8000; i++) {
                this.particle.spawnParticle(options);
            }
        }
        this.particle.update(this.inc);
    };

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
            this.tempObjects.forEach((to) => {
                to.position.set(newX, this.dest.y, newZ);
            })
            this.setChickenModel(-1);
        }, 25);
    }

    public setLevel(level: number) {
        if (level < 1 || level > 8)
            return ;
        const scale = 0.3 + (level * 0.5) / 8;
        this.object.scale.set(scale, scale, scale);
        this.tempObjects.forEach((to) => {
            to.scale.set(scale, scale, scale);
        })
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
            this.tempObjects.forEach((to) => {
                to.rotation.set(this.destRot.x, newRot, this.destRot.z);
            })
        }, 25);
    }

    public remove() {
        GUIManager.getInstance().getScene().remove(this.object);
        this.tempObjects.forEach((to) => {
            GUIManager.getInstance().getScene().remove(to);
        })
        this.removed = true;
        let audio = AudioManager.getInstance().getSound("chickenDeath");
        if (this.particleInterval)
            clearInterval(this.particleInterval);
        if (audio)
            audio.play();
        if (this.incantationSound && this.incantationSound.isPlaying)
            this.incantationSound.stop();
        let frame = this.inc;
        let tmpInterval = setInterval(() => {
            this.inc += this.clock.getDelta();
            this.particle.update(this.inc);
            if (this.inc - frame >= 1) {
                GUIManager.getInstance().getScene().remove((this.particle as any));
                clearInterval(tmpInterval);
            }
        }, 10);
    }

    public setParticle(state: boolean) {
        this.particleEnable = state;
        if (state && this.incantationSound) {
            this.incantationSound.setLoop(true);
            this.incantationSound.play();
        } else if (this.incantationSound && this.incantationSound.isPlaying) {
            this.incantationSound.stop();
        }
    }

    public setBroadcastBubble() {
        if (this.broadcastBubbleInterval) {
            clearInterval(this.broadcastBubbleInterval);
            this.broadcastBubbleInterval = undefined;
        }
        if (this.broadcastBubbleTimeout) {
            clearTimeout(this.broadcastBubbleTimeout);
            this.broadcastBubbleTimeout = undefined;
        }
        this.broadcastBubbleTexture.position.y = 8;
        this.broadcastBubbleTexture.rotation.set(0, -this.object.rotation.y, 0);
        if (this.broadcastBubbleTexture.material instanceof Material)
            this.broadcastBubbleTexture.material.transparent = true;
        this.object.add(this.broadcastBubbleTexture);
        this.broadcastBubbleTexture.scale.set(0.03, 0.03, 0.03);
        this.broadcastBubbleInterval = setInterval(() => {
            this.broadcastBubbleTexture.rotation.set(
                0, -this.object.rotation.y, 0,
            );
        }, 10);
        this.broadcastBubbleTimeout = setTimeout(() => {
            this.object.remove(this.broadcastBubbleTexture);
            if (this.broadcastBubbleInterval)
                clearInterval(this.broadcastBubbleInterval);
        }, 800);
    }

    public setChickenModel(nbr: number) {
        if (this.tempObjectsTimeout) {
            clearTimeout(this.tempObjectsTimeout);
            this.tempObjectsTimeout = undefined;
        }
        this.tempObjects.forEach((to) => {
            GUIManager.getInstance().getScene().remove(to);
        });
        if (nbr == -1) {
            if (!this.removed)
                GUIManager.getInstance().getScene().add(this.object);
        } else {
            GUIManager.getInstance().getScene().add(this.tempObjects[nbr]);
            this.tempObjectsTimeout = setTimeout(() => {
                this.tempObjects.forEach((to) => {
                    GUIManager.getInstance().getScene().remove(to);
                });
                if (!this.removed)
                    GUIManager.getInstance().getScene().add(this.object);
            }, 1000);
        }
    }

}