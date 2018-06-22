interface IGeneralCom {
    "type": string,
    "response-type": string
}

export interface IMapSize extends IGeneralCom {
    size: {
        width: number,
        height: number
    }
}

export interface ITileResp extends IGeneralCom {
    pos: {
        x: number,
        y: number
    },
    data: [{
        type: string,
        amount: number
    }]
}

export interface IDataResp {
    type: string,
    amount: number
}

interface IGeneralCommandCom {
    command: string
}

export interface ITileCommand extends IGeneralCommandCom {
    pos: {
        x: number,
        y: number
    }
}