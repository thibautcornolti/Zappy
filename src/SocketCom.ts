import * as io from "socket.io-client"

export default class SocketCom {
    private sock: SocketIOClient.Socket;
    private isError: boolean;

    constructor(port: number, host: string = "127.0.0.1", onConnect?: () => void, onError?: () => void, onData?: (resp: string) => void) {
        this.isError = false;
        this.sock = io.connect("http://" + host + ":" + port);

        if (onConnect)
            this.sock.on('connect', () => {
                setTimeout(() => {
                    if (!this.isError)
                        onConnect();
                    else if (onError)
                        onError();
                }, 200);
            });
        this.sock.on('my_error', (error: String) => {
            this.isError = true;
            this.onError(error);
        });
        if (onData)
            this.sock.on('my_data', onData);
    }

    public setOnData(fnt: (resp: string) => void) {
        this.sock.removeListener('my_data');
        this.sock.on('my_data', fnt);
    }

    private onError(error: String) {
        console.error(error);
    }

    private onData(msg: String) {
        // console.log(msg);
    }

    public send(str: String) {
        this.sock.emit('data', str);
    }
}