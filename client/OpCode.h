#ifndef _OP_CODE_H_
#define _OP_CODE_H_



enum AccountOpcode {
	ProtoBuff_Login = 100,//��¼
	ProtoBuff_Room_Operate,//�������
	ProtoBuff_Sync_Room_List,//ͬ�������б�
	ProtoBuff_Sync_Player_List,//ͬ���û��б�
	ProtoBuff_Add_Sharp,//����ͼԪ
	ProtoBuff_Remove_Sharp,//ɾ��ͼԪ
	ProtoBuff_Error_Info,//������Ϣ
};

#endif // _OP_CODE_H_