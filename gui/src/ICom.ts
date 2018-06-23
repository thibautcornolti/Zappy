interface IGeneralResp {
    "type": string,
    "response-type": string
}

export interface IMapSize extends IGeneralResp {
    size: {
        width: number,
        height: number
    }
}

export interface ITileResp extends IGeneralResp {
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

export interface IEntitiesResp extends IGeneralResp {
    data: [{
        type: string
        data: [{
            pos: {
                x: number,
                y: number,
            }
            amount: number
        }]
    }]
}

export interface IGeneralCommandCom {
    command: string
}

export interface ITileCommand extends IGeneralCommandCom {
    pos: {
        x: number,
        y: number
    }
}