# coding = utf-8
from src.classes.com.Controller import controller, Resources
from src.classes.com.SafeController import safe_controller
from src.classes.ia_res.Ant import ant
from src.classes.ia_res.MsgProtocol import MsgProtocol
from src.classes.ia_res.TrackableTransactions import LookTransaction, \
    BroadcastTransaction
from src.classes.states.SeekItemsState import SeekItemsState
from src.classes.states.StateMachine import AAIState, statemachine
from src.misc import my_print


class SlaveState(AAIState):

    def __init__(self):
        super().__init__("SlaveState")

    def find_callback(self, _):
        messages = controller.consultMessages()
        for m in messages:
            seek = MsgProtocol.is_seek_slave(m.text)
            if seek and seek['recipient'] == ant.uuid and seek['sender'] == ant.queen:
                my_print('I have finally a real duty!! :')
                my_print('{}'.format(seek['items']))
                self.queen_uuid = seek['sender']
                items = {
                    Resources(name): value
                    for name, value in seek['items'].items()
                }
                statemachine.closure = lambda: statemachine.push(
                    SeekItemsState(items))
                return
        find = LookTransaction(self.find_callback)
        safe_controller.execute(find)

    def meet_callback(self, _):
        messages = controller.consultMessages()
        for m in messages:
            meet = MsgProtocol.is_meet_ants(m.text)
            if meet and ant.uuid in meet['recipients'] and meet['sender'] == ant.queen:
                my_print("My dear queen asked me to join her, let's go!")
                # TODO: JOIN THE QUEEN WITH THE ANGLE OF THE BROADCAST LOL MDR
                return
        meet = LookTransaction(self.meet_callback)
        safe_controller.execute(meet)

    def on_push(self, cli):
        super().on_push(cli)
        find = LookTransaction(self.find_callback)
        safe_controller.execute(find)

    def popped_over(self):
        super().popped_over()

        def callback(_):
            meet = LookTransaction(self.meet_callback)
            safe_controller.execute(meet)

        my_print('I accomplished my duty ! :)')
        msg = MsgProtocol.seek_end(ant.uuid, ant.queen)
        transaction = BroadcastTransaction(msg, lambda: callback)
        safe_controller.execute(transaction)
