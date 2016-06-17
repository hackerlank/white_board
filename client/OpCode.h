#ifndef _OP_CODE_H_
#define _OP_CODE_H_



enum AccountOpcode {
	ProtoBuff_Login = 100,//登录
	ProtoBuff_Room_Operate,//房间操作
	ProtoBuff_Sync_Room_List,//同步房间列表
	ProtoBuff_Sync_Player_List,//同步用户列表
	ProtoBuff_Add_Sharp,//增加图元
	ProtoBuff_Remove_Sharp,//删除图元
	ProtoBuff_Error_Info,//错误信息
};

#endif // _OP_CODE_H_