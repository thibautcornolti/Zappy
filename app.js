#!/usr/bin/node

const net = require('net');
const process = require('process');

const app = require('express')();
const server = require('http').Server(app);
const io = require('socket.io')(server);
const serveStatic = require('serve-static');

let ipZappy = "127.0.0.1";
let portZappy = 333334;

process.argv.forEach((elem) => {
    if (elem.startsWith("--serv=")) {
        let servZappy = elem.split("--serv=")[1];
        ipZappy = servZappy.split(":")[0];
        portZappy = parseInt(servZappy.split(":")[1]);
    }
});


server.listen(33333);

app.use(serveStatic('public/', {'index': ['index.html', 'index.htm']}));

io.on('connection', function (sock) {
    console.log('Got connection');
    let cli = null;
    let isConnected = false;

    cli = net.connect(33334, "127.0.0.1", () => {
        isConnected = true;
    });

    cli.on('error', (err) => {
        isConnected = false;
        sock.emit('my_error', {code: err.code});
    });

    cli.on('data', (msg) => {
        sock.emit('my_data', msg.toString());
    });

    cli.on('close', () => {
        sock.emit('my_error', {code: "DISCONNECTED"});
    });

    sock.on('data', (msg) => {
        if (cli && isConnected) {
            cli.write(msg);
        } else {
            sock.emit('my_error', {code: "NOTCONNECTED"});
        }
    });

    sock.on('disconnect', () => {
        cli.end();
        isConnected = false;
        console.log('Got disconnection');
    });
});
