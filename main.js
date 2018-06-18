var scene = new THREE.Scene();
var camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
camera.position.z = 30;
camera.position.y = 40;
camera.lookAt(0, 0, 10);
var light = new THREE.AmbientLight(0xffffff, 0.7);
scene.add(light);
light = new THREE.DirectionalLight(0xffffff, 0.7);
light.position.set(0, 1, 0);
light.lookAt(0, 0, 0);
scene.add(light);
var onProgress = function (xhr) {
    if (xhr.lengthComputable) {
        var percentComplete = xhr.loaded / xhr.total * 100;
        console.log(Math.round(percentComplete) + '% downloaded');
    }
};
var onError = function (xhr) { };
THREE.Loader.Handlers.add(/\.dds$/i, new THREE.DDSLoader());
var mtlload = new THREE.MTLLoader();
mtlload.setPath('public/models/map/');
mtlload.load('map.mtl', function (materials) {
    materials.preload();
    var objload = new THREE.OBJLoader();
    objload.setMaterials(materials);
    objload.setPath('public/models/map/');
    objload.load('map.obj', function (object) {
        // object.position.y = - 95;
        object.rotation.y = (Math.PI / 2) * 3;
        console.log("ok");
        scene.add(object);
    }, onProgress, onError);
});
var renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);
// let geometry = new THREE.BoxGeometry( 1, 1, 1 );
// let material = new THREE.MeshBasicMaterial( { color: 0x00ff00 } );
// let cube = new THREE.Mesh( geometry, material );
// cube.matrixAutoUpdate = true;
// scene.add( cube );
// camera.position.z = 5;
var animate = function () {
    requestAnimationFrame(animate);
    // cube.rotation.x += 0.1;
    // cube.rotation.y += 0.1;
    renderer.render(scene, camera);
};
window.addEventListener('resize', onWindowResize, false);
function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
}
animate();
