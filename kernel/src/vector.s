/* Auto-generated interrupt vector table */
.intel_syntax noprefix

.section .rodata
.global trap_vectors
trap_vectors:
    .quad vector_0
    .quad vector_1
    .quad vector_2
    .quad vector_3
    .quad vector_4
    .quad vector_5
    .quad vector_6
    .quad vector_7
    .quad vector_8
    .quad vector_9
    .quad vector_10
    .quad vector_11
    .quad vector_12
    .quad vector_13
    .quad vector_14
    .quad vector_15
    .quad vector_16
    .quad vector_17
    .quad vector_18
    .quad vector_19
    .quad vector_20
    .quad vector_21
    .quad vector_22
    .quad vector_23
    .quad vector_24
    .quad vector_25
    .quad vector_26
    .quad vector_27
    .quad vector_28
    .quad vector_29
    .quad vector_30
    .quad vector_31
    .quad vector_32
    .quad vector_33
    .quad vector_34
    .quad vector_35
    .quad vector_36
    .quad vector_37
    .quad vector_38
    .quad vector_39
    .quad vector_40
    .quad vector_41
    .quad vector_42
    .quad vector_43
    .quad vector_44
    .quad vector_45
    .quad vector_46
    .quad vector_47
    .quad vector_48
    .quad vector_49
    .quad vector_50
    .quad vector_51
    .quad vector_52
    .quad vector_53
    .quad vector_54
    .quad vector_55
    .quad vector_56
    .quad vector_57
    .quad vector_58
    .quad vector_59
    .quad vector_60
    .quad vector_61
    .quad vector_62
    .quad vector_63
    .quad vector_64
    .quad vector_65
    .quad vector_66
    .quad vector_67
    .quad vector_68
    .quad vector_69
    .quad vector_70
    .quad vector_71
    .quad vector_72
    .quad vector_73
    .quad vector_74
    .quad vector_75
    .quad vector_76
    .quad vector_77
    .quad vector_78
    .quad vector_79
    .quad vector_80
    .quad vector_81
    .quad vector_82
    .quad vector_83
    .quad vector_84
    .quad vector_85
    .quad vector_86
    .quad vector_87
    .quad vector_88
    .quad vector_89
    .quad vector_90
    .quad vector_91
    .quad vector_92
    .quad vector_93
    .quad vector_94
    .quad vector_95
    .quad vector_96
    .quad vector_97
    .quad vector_98
    .quad vector_99
    .quad vector_100
    .quad vector_101
    .quad vector_102
    .quad vector_103
    .quad vector_104
    .quad vector_105
    .quad vector_106
    .quad vector_107
    .quad vector_108
    .quad vector_109
    .quad vector_110
    .quad vector_111
    .quad vector_112
    .quad vector_113
    .quad vector_114
    .quad vector_115
    .quad vector_116
    .quad vector_117
    .quad vector_118
    .quad vector_119
    .quad vector_120
    .quad vector_121
    .quad vector_122
    .quad vector_123
    .quad vector_124
    .quad vector_125
    .quad vector_126
    .quad vector_127
    .quad vector_128
    .quad vector_129
    .quad vector_130
    .quad vector_131
    .quad vector_132
    .quad vector_133
    .quad vector_134
    .quad vector_135
    .quad vector_136
    .quad vector_137
    .quad vector_138
    .quad vector_139
    .quad vector_140
    .quad vector_141
    .quad vector_142
    .quad vector_143
    .quad vector_144
    .quad vector_145
    .quad vector_146
    .quad vector_147
    .quad vector_148
    .quad vector_149
    .quad vector_150
    .quad vector_151
    .quad vector_152
    .quad vector_153
    .quad vector_154
    .quad vector_155
    .quad vector_156
    .quad vector_157
    .quad vector_158
    .quad vector_159
    .quad vector_160
    .quad vector_161
    .quad vector_162
    .quad vector_163
    .quad vector_164
    .quad vector_165
    .quad vector_166
    .quad vector_167
    .quad vector_168
    .quad vector_169
    .quad vector_170
    .quad vector_171
    .quad vector_172
    .quad vector_173
    .quad vector_174
    .quad vector_175
    .quad vector_176
    .quad vector_177
    .quad vector_178
    .quad vector_179
    .quad vector_180
    .quad vector_181
    .quad vector_182
    .quad vector_183
    .quad vector_184
    .quad vector_185
    .quad vector_186
    .quad vector_187
    .quad vector_188
    .quad vector_189
    .quad vector_190
    .quad vector_191
    .quad vector_192
    .quad vector_193
    .quad vector_194
    .quad vector_195
    .quad vector_196
    .quad vector_197
    .quad vector_198
    .quad vector_199
    .quad vector_200
    .quad vector_201
    .quad vector_202
    .quad vector_203
    .quad vector_204
    .quad vector_205
    .quad vector_206
    .quad vector_207
    .quad vector_208
    .quad vector_209
    .quad vector_210
    .quad vector_211
    .quad vector_212
    .quad vector_213
    .quad vector_214
    .quad vector_215
    .quad vector_216
    .quad vector_217
    .quad vector_218
    .quad vector_219
    .quad vector_220
    .quad vector_221
    .quad vector_222
    .quad vector_223
    .quad vector_224
    .quad vector_225
    .quad vector_226
    .quad vector_227
    .quad vector_228
    .quad vector_229
    .quad vector_230
    .quad vector_231
    .quad vector_232
    .quad vector_233
    .quad vector_234
    .quad vector_235
    .quad vector_236
    .quad vector_237
    .quad vector_238
    .quad vector_239
    .quad vector_240
    .quad vector_241
    .quad vector_242
    .quad vector_243
    .quad vector_244
    .quad vector_245
    .quad vector_246
    .quad vector_247
    .quad vector_248
    .quad vector_249
    .quad vector_250
    .quad vector_251
    .quad vector_252
    .quad vector_253
    .quad vector_254
    .quad vector_255

.section .text
.global vector_0
vector_0:
    push 0
    push 0
    jmp isr_common

.global vector_1
vector_1:
    push 0
    push 1
    jmp isr_common

.global vector_2
vector_2:
    push 0
    push 2
    jmp isr_common

.global vector_3
vector_3:
    push 0
    push 3
    jmp isr_common

.global vector_4
vector_4:
    push 0
    push 4
    jmp isr_common

.global vector_5
vector_5:
    push 0
    push 5
    jmp isr_common

.global vector_6
vector_6:
    push 0
    push 6
    jmp isr_common

.global vector_7
vector_7:
    push 0
    push 7
    jmp isr_common

.global vector_8
vector_8:
    push 8
    jmp isr_common

.global vector_9
vector_9:
    push 0
    push 9
    jmp isr_common

.global vector_10
vector_10:
    push 10
    jmp isr_common

.global vector_11
vector_11:
    push 11
    jmp isr_common

.global vector_12
vector_12:
    push 12
    jmp isr_common

.global vector_13
vector_13:
    push 13
    jmp isr_common

.global vector_14
vector_14:
    push 14
    jmp isr_common

.global vector_15
vector_15:
    push 0
    push 15
    jmp isr_common

.global vector_16
vector_16:
    push 0
    push 16
    jmp isr_common

.global vector_17
vector_17:
    push 17
    jmp isr_common

.global vector_18
vector_18:
    push 0
    push 18
    jmp isr_common

.global vector_19
vector_19:
    push 0
    push 19
    jmp isr_common

.global vector_20
vector_20:
    push 0
    push 20
    jmp isr_common

.global vector_21
vector_21:
    push 0
    push 21
    jmp isr_common

.global vector_22
vector_22:
    push 0
    push 22
    jmp isr_common

.global vector_23
vector_23:
    push 0
    push 23
    jmp isr_common

.global vector_24
vector_24:
    push 0
    push 24
    jmp isr_common

.global vector_25
vector_25:
    push 0
    push 25
    jmp isr_common

.global vector_26
vector_26:
    push 0
    push 26
    jmp isr_common

.global vector_27
vector_27:
    push 0
    push 27
    jmp isr_common

.global vector_28
vector_28:
    push 0
    push 28
    jmp isr_common

.global vector_29
vector_29:
    push 0
    push 29
    jmp isr_common

.global vector_30
vector_30:
    push 0
    push 30
    jmp isr_common

.global vector_31
vector_31:
    push 0
    push 31
    jmp isr_common

.global vector_32
vector_32:
    push 0
    push 32
    jmp isr_common

.global vector_33
vector_33:
    push 0
    push 33
    jmp isr_common

.global vector_34
vector_34:
    push 0
    push 34
    jmp isr_common

.global vector_35
vector_35:
    push 0
    push 35
    jmp isr_common

.global vector_36
vector_36:
    push 0
    push 36
    jmp isr_common

.global vector_37
vector_37:
    push 0
    push 37
    jmp isr_common

.global vector_38
vector_38:
    push 0
    push 38
    jmp isr_common

.global vector_39
vector_39:
    push 0
    push 39
    jmp isr_common

.global vector_40
vector_40:
    push 0
    push 40
    jmp isr_common

.global vector_41
vector_41:
    push 0
    push 41
    jmp isr_common

.global vector_42
vector_42:
    push 0
    push 42
    jmp isr_common

.global vector_43
vector_43:
    push 0
    push 43
    jmp isr_common

.global vector_44
vector_44:
    push 0
    push 44
    jmp isr_common

.global vector_45
vector_45:
    push 0
    push 45
    jmp isr_common

.global vector_46
vector_46:
    push 0
    push 46
    jmp isr_common

.global vector_47
vector_47:
    push 0
    push 47
    jmp isr_common

.global vector_48
vector_48:
    push 0
    push 48
    jmp isr_common

.global vector_49
vector_49:
    push 0
    push 49
    jmp isr_common

.global vector_50
vector_50:
    push 0
    push 50
    jmp isr_common

.global vector_51
vector_51:
    push 0
    push 51
    jmp isr_common

.global vector_52
vector_52:
    push 0
    push 52
    jmp isr_common

.global vector_53
vector_53:
    push 0
    push 53
    jmp isr_common

.global vector_54
vector_54:
    push 0
    push 54
    jmp isr_common

.global vector_55
vector_55:
    push 0
    push 55
    jmp isr_common

.global vector_56
vector_56:
    push 0
    push 56
    jmp isr_common

.global vector_57
vector_57:
    push 0
    push 57
    jmp isr_common

.global vector_58
vector_58:
    push 0
    push 58
    jmp isr_common

.global vector_59
vector_59:
    push 0
    push 59
    jmp isr_common

.global vector_60
vector_60:
    push 0
    push 60
    jmp isr_common

.global vector_61
vector_61:
    push 0
    push 61
    jmp isr_common

.global vector_62
vector_62:
    push 0
    push 62
    jmp isr_common

.global vector_63
vector_63:
    push 0
    push 63
    jmp isr_common

.global vector_64
vector_64:
    push 0
    push 64
    jmp isr_common

.global vector_65
vector_65:
    push 0
    push 65
    jmp isr_common

.global vector_66
vector_66:
    push 0
    push 66
    jmp isr_common

.global vector_67
vector_67:
    push 0
    push 67
    jmp isr_common

.global vector_68
vector_68:
    push 0
    push 68
    jmp isr_common

.global vector_69
vector_69:
    push 0
    push 69
    jmp isr_common

.global vector_70
vector_70:
    push 0
    push 70
    jmp isr_common

.global vector_71
vector_71:
    push 0
    push 71
    jmp isr_common

.global vector_72
vector_72:
    push 0
    push 72
    jmp isr_common

.global vector_73
vector_73:
    push 0
    push 73
    jmp isr_common

.global vector_74
vector_74:
    push 0
    push 74
    jmp isr_common

.global vector_75
vector_75:
    push 0
    push 75
    jmp isr_common

.global vector_76
vector_76:
    push 0
    push 76
    jmp isr_common

.global vector_77
vector_77:
    push 0
    push 77
    jmp isr_common

.global vector_78
vector_78:
    push 0
    push 78
    jmp isr_common

.global vector_79
vector_79:
    push 0
    push 79
    jmp isr_common

.global vector_80
vector_80:
    push 0
    push 80
    jmp isr_common

.global vector_81
vector_81:
    push 0
    push 81
    jmp isr_common

.global vector_82
vector_82:
    push 0
    push 82
    jmp isr_common

.global vector_83
vector_83:
    push 0
    push 83
    jmp isr_common

.global vector_84
vector_84:
    push 0
    push 84
    jmp isr_common

.global vector_85
vector_85:
    push 0
    push 85
    jmp isr_common

.global vector_86
vector_86:
    push 0
    push 86
    jmp isr_common

.global vector_87
vector_87:
    push 0
    push 87
    jmp isr_common

.global vector_88
vector_88:
    push 0
    push 88
    jmp isr_common

.global vector_89
vector_89:
    push 0
    push 89
    jmp isr_common

.global vector_90
vector_90:
    push 0
    push 90
    jmp isr_common

.global vector_91
vector_91:
    push 0
    push 91
    jmp isr_common

.global vector_92
vector_92:
    push 0
    push 92
    jmp isr_common

.global vector_93
vector_93:
    push 0
    push 93
    jmp isr_common

.global vector_94
vector_94:
    push 0
    push 94
    jmp isr_common

.global vector_95
vector_95:
    push 0
    push 95
    jmp isr_common

.global vector_96
vector_96:
    push 0
    push 96
    jmp isr_common

.global vector_97
vector_97:
    push 0
    push 97
    jmp isr_common

.global vector_98
vector_98:
    push 0
    push 98
    jmp isr_common

.global vector_99
vector_99:
    push 0
    push 99
    jmp isr_common

.global vector_100
vector_100:
    push 0
    push 100
    jmp isr_common

.global vector_101
vector_101:
    push 0
    push 101
    jmp isr_common

.global vector_102
vector_102:
    push 0
    push 102
    jmp isr_common

.global vector_103
vector_103:
    push 0
    push 103
    jmp isr_common

.global vector_104
vector_104:
    push 0
    push 104
    jmp isr_common

.global vector_105
vector_105:
    push 0
    push 105
    jmp isr_common

.global vector_106
vector_106:
    push 0
    push 106
    jmp isr_common

.global vector_107
vector_107:
    push 0
    push 107
    jmp isr_common

.global vector_108
vector_108:
    push 0
    push 108
    jmp isr_common

.global vector_109
vector_109:
    push 0
    push 109
    jmp isr_common

.global vector_110
vector_110:
    push 0
    push 110
    jmp isr_common

.global vector_111
vector_111:
    push 0
    push 111
    jmp isr_common

.global vector_112
vector_112:
    push 0
    push 112
    jmp isr_common

.global vector_113
vector_113:
    push 0
    push 113
    jmp isr_common

.global vector_114
vector_114:
    push 0
    push 114
    jmp isr_common

.global vector_115
vector_115:
    push 0
    push 115
    jmp isr_common

.global vector_116
vector_116:
    push 0
    push 116
    jmp isr_common

.global vector_117
vector_117:
    push 0
    push 117
    jmp isr_common

.global vector_118
vector_118:
    push 0
    push 118
    jmp isr_common

.global vector_119
vector_119:
    push 0
    push 119
    jmp isr_common

.global vector_120
vector_120:
    push 0
    push 120
    jmp isr_common

.global vector_121
vector_121:
    push 0
    push 121
    jmp isr_common

.global vector_122
vector_122:
    push 0
    push 122
    jmp isr_common

.global vector_123
vector_123:
    push 0
    push 123
    jmp isr_common

.global vector_124
vector_124:
    push 0
    push 124
    jmp isr_common

.global vector_125
vector_125:
    push 0
    push 125
    jmp isr_common

.global vector_126
vector_126:
    push 0
    push 126
    jmp isr_common

.global vector_127
vector_127:
    push 0
    push 127
    jmp isr_common

.global vector_128
vector_128:
    push 0
    push 128
    jmp isr_common

.global vector_129
vector_129:
    push 0
    push 129
    jmp isr_common

.global vector_130
vector_130:
    push 0
    push 130
    jmp isr_common

.global vector_131
vector_131:
    push 0
    push 131
    jmp isr_common

.global vector_132
vector_132:
    push 0
    push 132
    jmp isr_common

.global vector_133
vector_133:
    push 0
    push 133
    jmp isr_common

.global vector_134
vector_134:
    push 0
    push 134
    jmp isr_common

.global vector_135
vector_135:
    push 0
    push 135
    jmp isr_common

.global vector_136
vector_136:
    push 0
    push 136
    jmp isr_common

.global vector_137
vector_137:
    push 0
    push 137
    jmp isr_common

.global vector_138
vector_138:
    push 0
    push 138
    jmp isr_common

.global vector_139
vector_139:
    push 0
    push 139
    jmp isr_common

.global vector_140
vector_140:
    push 0
    push 140
    jmp isr_common

.global vector_141
vector_141:
    push 0
    push 141
    jmp isr_common

.global vector_142
vector_142:
    push 0
    push 142
    jmp isr_common

.global vector_143
vector_143:
    push 0
    push 143
    jmp isr_common

.global vector_144
vector_144:
    push 0
    push 144
    jmp isr_common

.global vector_145
vector_145:
    push 0
    push 145
    jmp isr_common

.global vector_146
vector_146:
    push 0
    push 146
    jmp isr_common

.global vector_147
vector_147:
    push 0
    push 147
    jmp isr_common

.global vector_148
vector_148:
    push 0
    push 148
    jmp isr_common

.global vector_149
vector_149:
    push 0
    push 149
    jmp isr_common

.global vector_150
vector_150:
    push 0
    push 150
    jmp isr_common

.global vector_151
vector_151:
    push 0
    push 151
    jmp isr_common

.global vector_152
vector_152:
    push 0
    push 152
    jmp isr_common

.global vector_153
vector_153:
    push 0
    push 153
    jmp isr_common

.global vector_154
vector_154:
    push 0
    push 154
    jmp isr_common

.global vector_155
vector_155:
    push 0
    push 155
    jmp isr_common

.global vector_156
vector_156:
    push 0
    push 156
    jmp isr_common

.global vector_157
vector_157:
    push 0
    push 157
    jmp isr_common

.global vector_158
vector_158:
    push 0
    push 158
    jmp isr_common

.global vector_159
vector_159:
    push 0
    push 159
    jmp isr_common

.global vector_160
vector_160:
    push 0
    push 160
    jmp isr_common

.global vector_161
vector_161:
    push 0
    push 161
    jmp isr_common

.global vector_162
vector_162:
    push 0
    push 162
    jmp isr_common

.global vector_163
vector_163:
    push 0
    push 163
    jmp isr_common

.global vector_164
vector_164:
    push 0
    push 164
    jmp isr_common

.global vector_165
vector_165:
    push 0
    push 165
    jmp isr_common

.global vector_166
vector_166:
    push 0
    push 166
    jmp isr_common

.global vector_167
vector_167:
    push 0
    push 167
    jmp isr_common

.global vector_168
vector_168:
    push 0
    push 168
    jmp isr_common

.global vector_169
vector_169:
    push 0
    push 169
    jmp isr_common

.global vector_170
vector_170:
    push 0
    push 170
    jmp isr_common

.global vector_171
vector_171:
    push 0
    push 171
    jmp isr_common

.global vector_172
vector_172:
    push 0
    push 172
    jmp isr_common

.global vector_173
vector_173:
    push 0
    push 173
    jmp isr_common

.global vector_174
vector_174:
    push 0
    push 174
    jmp isr_common

.global vector_175
vector_175:
    push 0
    push 175
    jmp isr_common

.global vector_176
vector_176:
    push 0
    push 176
    jmp isr_common

.global vector_177
vector_177:
    push 0
    push 177
    jmp isr_common

.global vector_178
vector_178:
    push 0
    push 178
    jmp isr_common

.global vector_179
vector_179:
    push 0
    push 179
    jmp isr_common

.global vector_180
vector_180:
    push 0
    push 180
    jmp isr_common

.global vector_181
vector_181:
    push 0
    push 181
    jmp isr_common

.global vector_182
vector_182:
    push 0
    push 182
    jmp isr_common

.global vector_183
vector_183:
    push 0
    push 183
    jmp isr_common

.global vector_184
vector_184:
    push 0
    push 184
    jmp isr_common

.global vector_185
vector_185:
    push 0
    push 185
    jmp isr_common

.global vector_186
vector_186:
    push 0
    push 186
    jmp isr_common

.global vector_187
vector_187:
    push 0
    push 187
    jmp isr_common

.global vector_188
vector_188:
    push 0
    push 188
    jmp isr_common

.global vector_189
vector_189:
    push 0
    push 189
    jmp isr_common

.global vector_190
vector_190:
    push 0
    push 190
    jmp isr_common

.global vector_191
vector_191:
    push 0
    push 191
    jmp isr_common

.global vector_192
vector_192:
    push 0
    push 192
    jmp isr_common

.global vector_193
vector_193:
    push 0
    push 193
    jmp isr_common

.global vector_194
vector_194:
    push 0
    push 194
    jmp isr_common

.global vector_195
vector_195:
    push 0
    push 195
    jmp isr_common

.global vector_196
vector_196:
    push 0
    push 196
    jmp isr_common

.global vector_197
vector_197:
    push 0
    push 197
    jmp isr_common

.global vector_198
vector_198:
    push 0
    push 198
    jmp isr_common

.global vector_199
vector_199:
    push 0
    push 199
    jmp isr_common

.global vector_200
vector_200:
    push 0
    push 200
    jmp isr_common

.global vector_201
vector_201:
    push 0
    push 201
    jmp isr_common

.global vector_202
vector_202:
    push 0
    push 202
    jmp isr_common

.global vector_203
vector_203:
    push 0
    push 203
    jmp isr_common

.global vector_204
vector_204:
    push 0
    push 204
    jmp isr_common

.global vector_205
vector_205:
    push 0
    push 205
    jmp isr_common

.global vector_206
vector_206:
    push 0
    push 206
    jmp isr_common

.global vector_207
vector_207:
    push 0
    push 207
    jmp isr_common

.global vector_208
vector_208:
    push 0
    push 208
    jmp isr_common

.global vector_209
vector_209:
    push 0
    push 209
    jmp isr_common

.global vector_210
vector_210:
    push 0
    push 210
    jmp isr_common

.global vector_211
vector_211:
    push 0
    push 211
    jmp isr_common

.global vector_212
vector_212:
    push 0
    push 212
    jmp isr_common

.global vector_213
vector_213:
    push 0
    push 213
    jmp isr_common

.global vector_214
vector_214:
    push 0
    push 214
    jmp isr_common

.global vector_215
vector_215:
    push 0
    push 215
    jmp isr_common

.global vector_216
vector_216:
    push 0
    push 216
    jmp isr_common

.global vector_217
vector_217:
    push 0
    push 217
    jmp isr_common

.global vector_218
vector_218:
    push 0
    push 218
    jmp isr_common

.global vector_219
vector_219:
    push 0
    push 219
    jmp isr_common

.global vector_220
vector_220:
    push 0
    push 220
    jmp isr_common

.global vector_221
vector_221:
    push 0
    push 221
    jmp isr_common

.global vector_222
vector_222:
    push 0
    push 222
    jmp isr_common

.global vector_223
vector_223:
    push 0
    push 223
    jmp isr_common

.global vector_224
vector_224:
    push 0
    push 224
    jmp isr_common

.global vector_225
vector_225:
    push 0
    push 225
    jmp isr_common

.global vector_226
vector_226:
    push 0
    push 226
    jmp isr_common

.global vector_227
vector_227:
    push 0
    push 227
    jmp isr_common

.global vector_228
vector_228:
    push 0
    push 228
    jmp isr_common

.global vector_229
vector_229:
    push 0
    push 229
    jmp isr_common

.global vector_230
vector_230:
    push 0
    push 230
    jmp isr_common

.global vector_231
vector_231:
    push 0
    push 231
    jmp isr_common

.global vector_232
vector_232:
    push 0
    push 232
    jmp isr_common

.global vector_233
vector_233:
    push 0
    push 233
    jmp isr_common

.global vector_234
vector_234:
    push 0
    push 234
    jmp isr_common

.global vector_235
vector_235:
    push 0
    push 235
    jmp isr_common

.global vector_236
vector_236:
    push 0
    push 236
    jmp isr_common

.global vector_237
vector_237:
    push 0
    push 237
    jmp isr_common

.global vector_238
vector_238:
    push 0
    push 238
    jmp isr_common

.global vector_239
vector_239:
    push 0
    push 239
    jmp isr_common

.global vector_240
vector_240:
    push 0
    push 240
    jmp isr_common

.global vector_241
vector_241:
    push 0
    push 241
    jmp isr_common

.global vector_242
vector_242:
    push 0
    push 242
    jmp isr_common

.global vector_243
vector_243:
    push 0
    push 243
    jmp isr_common

.global vector_244
vector_244:
    push 0
    push 244
    jmp isr_common

.global vector_245
vector_245:
    push 0
    push 245
    jmp isr_common

.global vector_246
vector_246:
    push 0
    push 246
    jmp isr_common

.global vector_247
vector_247:
    push 0
    push 247
    jmp isr_common

.global vector_248
vector_248:
    push 0
    push 248
    jmp isr_common

.global vector_249
vector_249:
    push 0
    push 249
    jmp isr_common

.global vector_250
vector_250:
    push 0
    push 250
    jmp isr_common

.global vector_251
vector_251:
    push 0
    push 251
    jmp isr_common

.global vector_252
vector_252:
    push 0
    push 252
    jmp isr_common

.global vector_253
vector_253:
    push 0
    push 253
    jmp isr_common

.global vector_254
vector_254:
    push 0
    push 254
    jmp isr_common

.global vector_255
vector_255:
    push 0
    push 255
    jmp isr_common

/*
 * Expected ABI for isr_common:
 *   stack top -> vector number
 */
.extern isr_common
