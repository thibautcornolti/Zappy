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