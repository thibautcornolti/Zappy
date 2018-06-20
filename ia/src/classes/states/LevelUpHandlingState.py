# coding = utf-8
import enum
from ia.src.classes.com.Controller import requirement, controller, defaultError
from ia.src.classes.ia_res.Ant import ant
from ia.src.classes.states.IncantationState import IncantationState
from ia.src.classes.states.SeekItemsState import SeekItemsState
from ia.src.classes.states.StateMachine import AAIState, statemachine


def transitionError():
    raise Exception("An unexpected transition occured")

class Status(enum.Enum):
    StandBy = 0
    Farming = 1
    Casting = 2

class LevelUpHandlingState(AAIState):

    def __init__(self):
        super().__init__("LevelHandler")
        self.lvl = ant.lvl + 1
        self.status = Status.StandBy

    # region Lvls event

    def aloneLvl(self, inventory):
        require = {k: max(v - inventory[k], 0) for k, v in requirement[self.lvl][1].items() if v > inventory[k]}
        statemachine.push(SeekItemsState(require))
        self.status = Status.Farming

    def teamLvl(self):
        pass
    # endregion

    # region transition events

    def endFarming(self):
        statemachine.push(IncantationState())

    # endregion

    # region inheritence

    def update(self, cli, inputs):
        super().update(cli, inputs)

    def on_push(self, cli):
        super().on_push(cli)
        if requirement[self.lvl][0] == 1:
            controller.inventory(self.aloneLvl)

    def popped_over(self):
        super().popped_over()
        calls = {
            Status.StandBy: transitionError,
            Status.Farming: self.endFarming
        }
        calls[self.status]()

    def on_pop(self, cli):
        super().on_pop(cli)

    # endregion inheritence