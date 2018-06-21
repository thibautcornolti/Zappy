# coding = utf-8
import enum
from src.classes.com.Controller import requirement, controller, defaultError
from src.classes.ia_res.Ant import ant
from src.classes.states.IncantationState import IncantationState
from src.classes.states.SeekItemsState import SeekItemsState
from src.classes.states.StateMachine import AAIState, statemachine


def transitionError():
    pass


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
        self.status = Status.Casting

    def endCasting(self, look=None):
        if look:
            ant.look = look
        controller.look(self.endCasting)

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
        print("TRANSITION")
        calls = {
            Status.StandBy: transitionError,
            Status.Farming: self.endFarming,
            Status.Casting: self.endCasting
        }
        status = self.status
        self.status = Status.StandBy
        calls[status]()

    def on_pop(self, cli):
        super().on_pop(cli)

    # endregion inheritence
