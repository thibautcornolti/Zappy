# coding = utf-8
from src.classes.com.Controller import Resources
from src.classes.ia_res.Ant import ant
from src.classes.ia_res.TrackableTransactions import InventoryTransaction
from src.classes.states.StateMachine import statemachine
from src.misc import my_log, my_print


class SafeController(object):

    def __init__(self):
        self.rollback = True
        self.floor = None
        self.upfloor = 20
        self.save = None
        self.endTransaction = None
        self.safe = True

    def clear_transaction(self, *args, **kwargs):
        endTransa = self.endTransaction
        self.save = None
        self.floor = None
        self.endTransaction = None
        endTransa(*args, **kwargs)

    def clear_state(self, cli):
        del cli
        my_print("END EMERGENCY")
        def reset_trans():
            statemachine.block_trans_detect = False

        statemachine.closure = reset_trans
        self.safe = True
        self.safe_exec(self.save, self.floor, self.rollback)

    def estimate_food(self, inventory):
        from src.classes.states.SeekItemsState import SeekItemsState

        #if Resources.Food not in ant.inventory or ant.inventory[Resources.Food] != inventory[Resources.Food]:
            #my_print("Food lvl : ", inventory[Resources.Food])
        ant.inventory = inventory
        if self.floor >= 0:
                state = None
                if inventory[Resources.Food] < self.save.get_estimated_time() and self.save.get_estimated_time() < self.floor:
                    a = self.upfloor
                    b = self.save.get_estimated_time()
                    val = a if a > b else b
                    state = SeekItemsState({Resources.Food: int(val)}, self.rollback)
                elif inventory[Resources.Food] < self.floor:
                    state = SeekItemsState({Resources.Food: int(self.floor) + self.upfloor - self.floor},  self.rollback)
                if state:
                    my_print("EMERGENCY")
                    statemachine.block_trans_detect = True
                    self.safe = False
                    state.on_pop = self.clear_state
                    statemachine.push(state)
                    return
        self.endTransaction = self.save.end
        self.save.end = self.clear_transaction
        self.save.execute()

    def safe_exec(self, transaction, floor, rollback):
        self.save = transaction
        self.floor = floor
        self.rollback = rollback
        InventoryTransaction(self.estimate_food).execute()

    def execute(self, transaction, floor=15, rollback=True):
        if self.save and self.safe:
            raise Exception("Invalid Concurrent transaction : {} vs {}".format(self.save.__repr__(), transaction.__repr__()))
        if self.safe:
            self.safe_exec(transaction, floor, rollback)
        else:
            transaction.execute()


safe_controller = SafeController()
