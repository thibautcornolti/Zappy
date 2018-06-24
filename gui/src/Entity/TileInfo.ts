import {IPlayerEntity, ITileResp} from "../ICom";

let elementTable: { [index: string]: { name: string, img: string } } = {
    linemate: {
        name: "Charbon (Linemate)",
        img: "/images/coal.png"
    },
    deraumere: {
        name: "Fer (Deraumere)",
        img: "/images/iron.png"
    },
    sibur: {
        name: "Lapis (Sibur)",
        img: "/images/lapis.png"
    },
    mendiane: {
        name: "Or (Mendiane)",
        img: "/images/gold.png"
    },
    phiras: {
        name: "Emeraude (Phiras)",
        img: "/images/emerald.png"
    },
    thystame: {
        name: "Diamant (Thystame)",
        img: "/images/diamond.png"
    },
    food: {
        name: "Nourriture (Food)",
        img: "/images/food.png"
    },
    // player: {
    //     name: "chicken",
    //     id: 7
    // }
};

export default class TileInfo {
    private background: HTMLElementTagNameMap["div"];

    constructor() {
        this.background = document.createElement('div');
        this.background.classList.add("tileInfo-bg");

        document.body.appendChild(this.background);
    }

    public show() {
        this.background.classList.add("strafe");
    }

    private setPositionQuery(data: ITileResp) {
        let mainDiv = document.createElement('div');
        let header = document.createElement('span');
        let content = document.createElement('span');

        header.innerText = "Position: ";
        content.innerText = "[" + (data.pos.x + 1) + ";" + (data.pos.y + 1) + "]";
        mainDiv.classList.add("position");
        mainDiv.appendChild(header);
        mainDiv.appendChild(content);
        this.background.appendChild(mainDiv);
    }

    private setElementInfo(data: ITileResp) {
        let component = document.createElement('div');

        data.data.forEach((elem) => {
            let info = elementTable[elem.type];
            if (!info)
                return;
            let mainDiv = document.createElement('div');
            let header = document.createElement('span');
            let img = document.createElement('img');
            let content = document.createElement('span');

            header.innerText = info.name + ": ";
            img.src = info.img;
            img.style.width = "40px";
            content.innerText = elem.amount + "";
            mainDiv.classList.add("item");
            mainDiv.appendChild(img);
            mainDiv.appendChild(header);
            mainDiv.appendChild(content);
            component.appendChild(mainDiv);
        });
        component.classList.add("items");
        this.background.appendChild(component);
    }

    public addPlayerInfo(data: IPlayerEntity) {
        let elem = document.querySelector('.players');
        if (!elem)
            return;
        elem.classList.remove("hide");

        let mainDiv = document.createElement('div');
        let header = document.createElement('span');
        let img = document.createElement('img');
        let content = document.createElement('span');

        header.innerText = 'Player: ';
        img.src = "/images/player.png";
        img.style.width = "40px";
        content.innerText = data.team + " (Niveau: " + data.level + ")";
        mainDiv.classList.add("item");
        mainDiv.appendChild(img);
        mainDiv.appendChild(header);
        mainDiv.appendChild(content);
        elem.appendChild(mainDiv);
        console.log(data);
    }

    public setInfo(data: ITileResp) {
        this.background.innerHTML = "";
        this.setPositionQuery(data);
        this.setElementInfo(data);
        let players = document.createElement('div');
        players.classList.add("players");
        players.classList.add("hide");
        this.background.appendChild(players);
    }

    public hide() {
        this.background.classList.remove("strafe");
    }
}