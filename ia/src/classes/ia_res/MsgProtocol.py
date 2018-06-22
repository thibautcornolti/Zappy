import itertools
import re


class MsgProtocol:

    @staticmethod
    def _is_template(group_names, regex, msg):
        def get_gn_name(gn):
            return gn[0] if isinstance(gn, tuple) else gn

        def get_gn_lambda(gn):
            return gn[1] if isinstance(gn, tuple) else lambda x: x

        res = re.findall(regex, msg)
        return {
            get_gn_name(n): get_gn_lambda(n)(res[0][i])
            for i, n in zip(itertools.count(), group_names)
        } if len(res) > 0 and len(res[0]) == len(group_names) else False

    @staticmethod
    def get_uuid(msg):
        res = re.findall(r"^[0-9a-z]*", msg)
        return res[0] if len(res) else None

    @staticmethod
    def enrolment(self_uuid, lvl_required):
        return "%s | Vend place PL 42sh!! No noob lvl %d plz" % (
            self_uuid, lvl_required)

    @staticmethod
    def is_enrolment(msg):
        return MsgProtocol._is_template(
            ('sender', 'level'),
            r"^([0-9a-z]+) \| Vend place PL 42sh!! No noob lvl ([3-8]) plz$",
            msg
        )

    @staticmethod
    def apply(self_uuid, dest_uuid):
        return "%s | stp %s achete 5 kama avant apres !! no fak" % (
            self_uuid, dest_uuid)

    @staticmethod
    def is_apply(msg):
        return MsgProtocol._is_template(
            ('sender', 'recipient'),
            r"^([0-9a-z]+) \| stp ([0-9a-z]+) achete 5 kama avant apres !! no fak$",
            msg
        )

    @staticmethod
    def allowed_ants(self_uuid, allowed_uuid):
        return "%s | mes esclaves %s" % (
            self_uuid, ','.join(allowed_uuid))

    @staticmethod
    def is_allowed_ants(msg):
        return MsgProtocol._is_template(
            ('sender', ('allowed_ants', lambda x: x.split(','))),
            r"^([0-9a-z]+) \| mes esclaves ([0-9a-z,]*)",
            msg
        )

    @staticmethod
    def seek_slave(self_uuid, dest_uuid, items):
        return "%s | va chercher %s : %s" % (
            self_uuid, dest_uuid,
            ','.join(["%s %s" % (k, v) for k, v in items.items()]))

    @staticmethod
    def is_seek_slave(msg):
        return MsgProtocol._is_template(
            ('sender', 'recipient', ('items', lambda x: {
                o.split(' ')[0]: o.split(' ')[1]
                for o in x.split(',')
            })),
            r"^([0-9a-z]+) \| va chercher ([0-9a-z]+) : ([a-z0-9_ ,]*)",
            msg
        )


if __name__ == '__main__':
    fake_uuid = '48m0s5x2'
    fake_dest = fake_uuid[::-1]
    fake_lvl = 4

    print('ENROLMENT')
    enrolment_msg = MsgProtocol.enrolment(fake_uuid, 8)
    print(enrolment_msg)
    print(MsgProtocol.is_enrolment(enrolment_msg))
    print()

    print('APPLY:')
    apply_msg = MsgProtocol.apply(fake_uuid, fake_dest)
    print(apply_msg)
    print(MsgProtocol.is_apply(apply_msg))
    print()

    print('ALLOWED ANTS:')
    allowed_ants_msg = MsgProtocol.allowed_ants(fake_uuid, [fake_dest] * 4)
    print(allowed_ants_msg)
    print(MsgProtocol.is_allowed_ants(allowed_ants_msg))
    print()

    print('SEEK SLAVE:')
    seek_slave_msg = MsgProtocol.seek_slave(fake_uuid, fake_dest,
                                            {'fake_item_1': 45,
                                             'fake_item_2': 39,
                                             'fake_item_3': 2,
                                             'fake_item_4': 93})
    print(seek_slave_msg)
    print(MsgProtocol.is_seek_slave(seek_slave_msg))
    print()
