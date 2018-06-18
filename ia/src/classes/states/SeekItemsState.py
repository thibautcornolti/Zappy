from ia.src.classes.states.StateMachine import AAIState
#from ia.src.classes.ia_res.Ant import ant
from ia.src.classes.com.Controller import controller

class SeekItemsState(AAIState):

    def updateAntLook(self, look):
        pass

    def update_out(self, cli):
        controller.look(self.updateAntLook)

    def __init__(self, items_list, food=False):
        super().__init__("SeekItems")
        self.looked = False
        self.items_list = items_list
        self.food = food
