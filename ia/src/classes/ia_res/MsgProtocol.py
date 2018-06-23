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

    @staticmethod
    def seek_end(self_uuid, dest_uuid):
        return "%s | j'ai fini reine %s" % (
            self_uuid, dest_uuid)

    @staticmethod
    def is_seek_end(msg):
        return MsgProtocol._is_template(
            ('sender', 'recipient'),
            r"^([0-9a-z]+) \| j'ai fini reine ([0-9a-z]+)",
            msg
        )

    @staticmethod
    def meet_ants(self_uuid, dest_uuids):
        return "%s | venez a moi bande d'insecte %s" % (
            self_uuid, ','.join(dest_uuids))

    @staticmethod
    def is_meet_ants(msg):
        return MsgProtocol._is_template(
            ('sender', ('recipients', lambda x: x.split(','))),
            r"^([0-9a-z]+) \| venez a moi bande d'insecte ([0-9a-z,]*)",
            msg
        )


if __name__ == '__main__':
    def test(test_name, fct, is_fct, args):
        header = '===== %s =====' % test_name
        print(header)
        msg = fct(*args)
        print(msg)
        print(is_fct(msg))
        print('=' * len(header))
        print()


    fake_uuid = '48m0s5x2'
    fake_dest = fake_uuid[::-1]
    fake_lvl = 4

    args = [fake_uuid, fake_lvl]
    test('ENROLMENT', MsgProtocol.enrolment, MsgProtocol.is_enrolment, args)

    args = [fake_uuid, fake_dest]
    test('APPLY', MsgProtocol.apply, MsgProtocol.is_apply, args)

    args = [fake_uuid, [fake_dest] * 4]
    test('ALLOWED ANTS', MsgProtocol.allowed_ants, MsgProtocol.is_allowed_ants,
         args)

    args = [fake_uuid, fake_dest,
            {'fake_item_1': 45,
             'fake_item_2': 39,
             'fake_item_3': 2,
             'fake_item_4': 93}]
    test('SEEK SLAVE', MsgProtocol.seek_slave, MsgProtocol.is_seek_slave, args)

    args = [fake_uuid, fake_dest]
    test('SEEK END', MsgProtocol.seek_end, MsgProtocol.is_seek_end, args)

    args = [fake_uuid, [fake_dest] * 4]
    test('MEET ANTS', MsgProtocol.meet_ants, MsgProtocol.is_meet_ants, args)
