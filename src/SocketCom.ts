// import net, { Socket } from "net"

import io from "socket.io-client";

// import SocketIOClient from "socket.io-client"

export default class SocketCom {
    private sock: SocketIOClient.Socket;

    constructor(port : number, host : string = "127.0.0.1") {
        console.log("http://" + host + ":" + port);
        this.sock = io.connect("http://" + host + ":" + port);

        this.sock.on('my_error', this.onError);
        this.sock.on('my_data', this.onData);
    }

    onError(error : String) {
        console.error(error);
    }

    onData(msg : String) {
        console.log(msg);
    }

    send(str: String) {
        this.sock.emit('data', str);
    }
}