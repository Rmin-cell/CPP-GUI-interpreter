// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.36.4
// 	protoc        v3.14.0
// source: mathexpr.proto

package proto

import (
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
	unsafe "unsafe"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

type ExprRequest struct {
	state         protoimpl.MessageState `protogen:"open.v1"`
	Expression    string                 `protobuf:"bytes,1,opt,name=expression,proto3" json:"expression,omitempty"`
	unknownFields protoimpl.UnknownFields
	sizeCache     protoimpl.SizeCache
}

func (x *ExprRequest) Reset() {
	*x = ExprRequest{}
	mi := &file_mathexpr_proto_msgTypes[0]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *ExprRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*ExprRequest) ProtoMessage() {}

func (x *ExprRequest) ProtoReflect() protoreflect.Message {
	mi := &file_mathexpr_proto_msgTypes[0]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use ExprRequest.ProtoReflect.Descriptor instead.
func (*ExprRequest) Descriptor() ([]byte, []int) {
	return file_mathexpr_proto_rawDescGZIP(), []int{0}
}

func (x *ExprRequest) GetExpression() string {
	if x != nil {
		return x.Expression
	}
	return ""
}

type VariablesListReply struct {
	state         protoimpl.MessageState `protogen:"open.v1"`
	Tokens        []string               `protobuf:"bytes,1,rep,name=tokens,proto3" json:"tokens,omitempty"`
	unknownFields protoimpl.UnknownFields
	sizeCache     protoimpl.SizeCache
}

func (x *VariablesListReply) Reset() {
	*x = VariablesListReply{}
	mi := &file_mathexpr_proto_msgTypes[1]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *VariablesListReply) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*VariablesListReply) ProtoMessage() {}

func (x *VariablesListReply) ProtoReflect() protoreflect.Message {
	mi := &file_mathexpr_proto_msgTypes[1]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use VariablesListReply.ProtoReflect.Descriptor instead.
func (*VariablesListReply) Descriptor() ([]byte, []int) {
	return file_mathexpr_proto_rawDescGZIP(), []int{1}
}

func (x *VariablesListReply) GetTokens() []string {
	if x != nil {
		return x.Tokens
	}
	return nil
}

type VariableWithRange struct {
	state         protoimpl.MessageState `protogen:"open.v1"`
	VariableName  string                 `protobuf:"bytes,1,opt,name=variable_name,json=variableName,proto3" json:"variable_name,omitempty"`
	Min           float64                `protobuf:"fixed64,2,opt,name=min,proto3" json:"min,omitempty"`
	Max           float64                `protobuf:"fixed64,3,opt,name=max,proto3" json:"max,omitempty"`
	unknownFields protoimpl.UnknownFields
	sizeCache     protoimpl.SizeCache
}

func (x *VariableWithRange) Reset() {
	*x = VariableWithRange{}
	mi := &file_mathexpr_proto_msgTypes[2]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *VariableWithRange) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*VariableWithRange) ProtoMessage() {}

func (x *VariableWithRange) ProtoReflect() protoreflect.Message {
	mi := &file_mathexpr_proto_msgTypes[2]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use VariableWithRange.ProtoReflect.Descriptor instead.
func (*VariableWithRange) Descriptor() ([]byte, []int) {
	return file_mathexpr_proto_rawDescGZIP(), []int{2}
}

func (x *VariableWithRange) GetVariableName() string {
	if x != nil {
		return x.VariableName
	}
	return ""
}

func (x *VariableWithRange) GetMin() float64 {
	if x != nil {
		return x.Min
	}
	return 0
}

func (x *VariableWithRange) GetMax() float64 {
	if x != nil {
		return x.Max
	}
	return 0
}

type DataPointsRequest struct {
	state          protoimpl.MessageState `protogen:"open.v1"`
	Expression     string                 `protobuf:"bytes,1,opt,name=expression,proto3" json:"expression,omitempty"`
	VariableRanges []*VariableWithRange   `protobuf:"bytes,2,rep,name=variable_ranges,json=variableRanges,proto3" json:"variable_ranges,omitempty"`
	unknownFields  protoimpl.UnknownFields
	sizeCache      protoimpl.SizeCache
}

func (x *DataPointsRequest) Reset() {
	*x = DataPointsRequest{}
	mi := &file_mathexpr_proto_msgTypes[3]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *DataPointsRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*DataPointsRequest) ProtoMessage() {}

func (x *DataPointsRequest) ProtoReflect() protoreflect.Message {
	mi := &file_mathexpr_proto_msgTypes[3]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use DataPointsRequest.ProtoReflect.Descriptor instead.
func (*DataPointsRequest) Descriptor() ([]byte, []int) {
	return file_mathexpr_proto_rawDescGZIP(), []int{3}
}

func (x *DataPointsRequest) GetExpression() string {
	if x != nil {
		return x.Expression
	}
	return ""
}

func (x *DataPointsRequest) GetVariableRanges() []*VariableWithRange {
	if x != nil {
		return x.VariableRanges
	}
	return nil
}

type DataPointsReply struct {
	state         protoimpl.MessageState `protogen:"open.v1"`
	Points        []float64              `protobuf:"fixed64,1,rep,packed,name=points,proto3" json:"points,omitempty"`
	unknownFields protoimpl.UnknownFields
	sizeCache     protoimpl.SizeCache
}

func (x *DataPointsReply) Reset() {
	*x = DataPointsReply{}
	mi := &file_mathexpr_proto_msgTypes[4]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *DataPointsReply) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*DataPointsReply) ProtoMessage() {}

func (x *DataPointsReply) ProtoReflect() protoreflect.Message {
	mi := &file_mathexpr_proto_msgTypes[4]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use DataPointsReply.ProtoReflect.Descriptor instead.
func (*DataPointsReply) Descriptor() ([]byte, []int) {
	return file_mathexpr_proto_rawDescGZIP(), []int{4}
}

func (x *DataPointsReply) GetPoints() []float64 {
	if x != nil {
		return x.Points
	}
	return nil
}

var File_mathexpr_proto protoreflect.FileDescriptor

var file_mathexpr_proto_rawDesc = string([]byte{
	0x0a, 0x0e, 0x6d, 0x61, 0x74, 0x68, 0x65, 0x78, 0x70, 0x72, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f,
	0x12, 0x0e, 0x6d, 0x61, 0x74, 0x68, 0x65, 0x78, 0x70, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6f, 0x6e,
	0x22, 0x2d, 0x0a, 0x0b, 0x45, 0x78, 0x70, 0x72, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x12,
	0x1e, 0x0a, 0x0a, 0x65, 0x78, 0x70, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x18, 0x01, 0x20,
	0x01, 0x28, 0x09, 0x52, 0x0a, 0x65, 0x78, 0x70, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x22,
	0x2c, 0x0a, 0x12, 0x56, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x73, 0x4c, 0x69, 0x73, 0x74,
	0x52, 0x65, 0x70, 0x6c, 0x79, 0x12, 0x16, 0x0a, 0x06, 0x74, 0x6f, 0x6b, 0x65, 0x6e, 0x73, 0x18,
	0x01, 0x20, 0x03, 0x28, 0x09, 0x52, 0x06, 0x74, 0x6f, 0x6b, 0x65, 0x6e, 0x73, 0x22, 0x5c, 0x0a,
	0x11, 0x56, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x57, 0x69, 0x74, 0x68, 0x52, 0x61, 0x6e,
	0x67, 0x65, 0x12, 0x23, 0x0a, 0x0d, 0x76, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x5f, 0x6e,
	0x61, 0x6d, 0x65, 0x18, 0x01, 0x20, 0x01, 0x28, 0x09, 0x52, 0x0c, 0x76, 0x61, 0x72, 0x69, 0x61,
	0x62, 0x6c, 0x65, 0x4e, 0x61, 0x6d, 0x65, 0x12, 0x10, 0x0a, 0x03, 0x6d, 0x69, 0x6e, 0x18, 0x02,
	0x20, 0x01, 0x28, 0x01, 0x52, 0x03, 0x6d, 0x69, 0x6e, 0x12, 0x10, 0x0a, 0x03, 0x6d, 0x61, 0x78,
	0x18, 0x03, 0x20, 0x01, 0x28, 0x01, 0x52, 0x03, 0x6d, 0x61, 0x78, 0x22, 0x7f, 0x0a, 0x11, 0x44,
	0x61, 0x74, 0x61, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x73, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74,
	0x12, 0x1e, 0x0a, 0x0a, 0x65, 0x78, 0x70, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x18, 0x01,
	0x20, 0x01, 0x28, 0x09, 0x52, 0x0a, 0x65, 0x78, 0x70, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6f, 0x6e,
	0x12, 0x4a, 0x0a, 0x0f, 0x76, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x5f, 0x72, 0x61, 0x6e,
	0x67, 0x65, 0x73, 0x18, 0x02, 0x20, 0x03, 0x28, 0x0b, 0x32, 0x21, 0x2e, 0x6d, 0x61, 0x74, 0x68,
	0x65, 0x78, 0x70, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x2e, 0x56, 0x61, 0x72, 0x69, 0x61,
	0x62, 0x6c, 0x65, 0x57, 0x69, 0x74, 0x68, 0x52, 0x61, 0x6e, 0x67, 0x65, 0x52, 0x0e, 0x76, 0x61,
	0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x52, 0x61, 0x6e, 0x67, 0x65, 0x73, 0x22, 0x29, 0x0a, 0x0f,
	0x44, 0x61, 0x74, 0x61, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x73, 0x52, 0x65, 0x70, 0x6c, 0x79, 0x12,
	0x16, 0x0a, 0x06, 0x70, 0x6f, 0x69, 0x6e, 0x74, 0x73, 0x18, 0x01, 0x20, 0x03, 0x28, 0x01, 0x52,
	0x06, 0x70, 0x6f, 0x69, 0x6e, 0x74, 0x73, 0x32, 0xbe, 0x01, 0x0a, 0x0e, 0x4d, 0x61, 0x74, 0x68,
	0x45, 0x78, 0x70, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x12, 0x55, 0x0a, 0x10, 0x67, 0x65,
	0x74, 0x56, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x73, 0x4c, 0x69, 0x73, 0x74, 0x12, 0x1b,
	0x2e, 0x6d, 0x61, 0x74, 0x68, 0x65, 0x78, 0x70, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x2e,
	0x45, 0x78, 0x70, 0x72, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x1a, 0x22, 0x2e, 0x6d, 0x61,
	0x74, 0x68, 0x65, 0x78, 0x70, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x2e, 0x56, 0x61, 0x72,
	0x69, 0x61, 0x62, 0x6c, 0x65, 0x73, 0x4c, 0x69, 0x73, 0x74, 0x52, 0x65, 0x70, 0x6c, 0x79, 0x22,
	0x00, 0x12, 0x55, 0x0a, 0x0d, 0x67, 0x65, 0x74, 0x44, 0x61, 0x74, 0x61, 0x50, 0x6f, 0x69, 0x6e,
	0x74, 0x73, 0x12, 0x21, 0x2e, 0x6d, 0x61, 0x74, 0x68, 0x65, 0x78, 0x70, 0x72, 0x65, 0x73, 0x73,
	0x69, 0x6f, 0x6e, 0x2e, 0x44, 0x61, 0x74, 0x61, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x73, 0x52, 0x65,
	0x71, 0x75, 0x65, 0x73, 0x74, 0x1a, 0x1f, 0x2e, 0x6d, 0x61, 0x74, 0x68, 0x65, 0x78, 0x70, 0x72,
	0x65, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x2e, 0x44, 0x61, 0x74, 0x61, 0x50, 0x6f, 0x69, 0x6e, 0x74,
	0x73, 0x52, 0x65, 0x70, 0x6c, 0x79, 0x22, 0x00, 0x42, 0x09, 0x5a, 0x07, 0x2e, 0x2f, 0x70, 0x72,
	0x6f, 0x74, 0x6f, 0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
})

var (
	file_mathexpr_proto_rawDescOnce sync.Once
	file_mathexpr_proto_rawDescData []byte
)

func file_mathexpr_proto_rawDescGZIP() []byte {
	file_mathexpr_proto_rawDescOnce.Do(func() {
		file_mathexpr_proto_rawDescData = protoimpl.X.CompressGZIP(unsafe.Slice(unsafe.StringData(file_mathexpr_proto_rawDesc), len(file_mathexpr_proto_rawDesc)))
	})
	return file_mathexpr_proto_rawDescData
}

var file_mathexpr_proto_msgTypes = make([]protoimpl.MessageInfo, 5)
var file_mathexpr_proto_goTypes = []any{
	(*ExprRequest)(nil),        // 0: mathexpression.ExprRequest
	(*VariablesListReply)(nil), // 1: mathexpression.VariablesListReply
	(*VariableWithRange)(nil),  // 2: mathexpression.VariableWithRange
	(*DataPointsRequest)(nil),  // 3: mathexpression.DataPointsRequest
	(*DataPointsReply)(nil),    // 4: mathexpression.DataPointsReply
}
var file_mathexpr_proto_depIdxs = []int32{
	2, // 0: mathexpression.DataPointsRequest.variable_ranges:type_name -> mathexpression.VariableWithRange
	0, // 1: mathexpression.MathExpression.getVariablesList:input_type -> mathexpression.ExprRequest
	3, // 2: mathexpression.MathExpression.getDataPoints:input_type -> mathexpression.DataPointsRequest
	1, // 3: mathexpression.MathExpression.getVariablesList:output_type -> mathexpression.VariablesListReply
	4, // 4: mathexpression.MathExpression.getDataPoints:output_type -> mathexpression.DataPointsReply
	3, // [3:5] is the sub-list for method output_type
	1, // [1:3] is the sub-list for method input_type
	1, // [1:1] is the sub-list for extension type_name
	1, // [1:1] is the sub-list for extension extendee
	0, // [0:1] is the sub-list for field type_name
}

func init() { file_mathexpr_proto_init() }
func file_mathexpr_proto_init() {
	if File_mathexpr_proto != nil {
		return
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: unsafe.Slice(unsafe.StringData(file_mathexpr_proto_rawDesc), len(file_mathexpr_proto_rawDesc)),
			NumEnums:      0,
			NumMessages:   5,
			NumExtensions: 0,
			NumServices:   1,
		},
		GoTypes:           file_mathexpr_proto_goTypes,
		DependencyIndexes: file_mathexpr_proto_depIdxs,
		MessageInfos:      file_mathexpr_proto_msgTypes,
	}.Build()
	File_mathexpr_proto = out.File
	file_mathexpr_proto_goTypes = nil
	file_mathexpr_proto_depIdxs = nil
}
