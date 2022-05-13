from typing import OrderedDict
import torch.nn as nn
import torch.nn.functional as F
from D3_layers import FullyCon3D, Pool3D, SoftMax3D

class GRU_Encoder(nn.Module):
    '''
    нууу, енкодер это енкодер
    '''
    def __init__(self, input_shape, conv_shapes, fc_shapes, conv3_shape, out_shape, res=True):

        if (len(conv_shapes) != 5):
            print("ERROR")
            return None

        super().__init__()

        self.res = res # по умолчанию наверное residual норм    
        self.relu = nn.LeakyReLU(negative_slope=0.01)
        self.pool = nn.MaxPool2d(2, stride=1)

        # будет использовано, если не подойдет residual архитектура

        self.head = nn.Sequential(OrderedDict([
            ('conv1', nn.Conv2d(input_shape[1], conv_shapes[0], 7, padding=3)),
            ('pool1', self.pool),
            ('relu1', self.relu),
            ('conv2', nn.Conv2d(conv_shapes[0], conv_shapes[1], 3, padding=1)),
            ('pool2', self.pool),
            ('relu2', self.relu),
            ('conv3', nn.Conv2d(conv_shapes[1], conv_shapes[2], 3, padding=1)),
            ('pool3', self.pool),
            ('relu3', self.relu),
            ('conv4', nn.Conv2d(conv_shapes[2], conv_shapes[3], 3, padding=1)),
            ('pool4', self.pool),
            ('relu4', self.relu),
            ('conv5', nn.Conv2d(conv_shapes[3], conv_shapes[4], 3, padding=1)),
            ('pool5', self.pool),
            ('relu5', self.relu),
            ('conv6', nn.Conv2d(conv_shapes[4], conv_shapes[5], 3, padding=1)),
            ('pool6', self.pool),
            ('relu6', self.relu),
        ]))

        # inseption модули для res-архитекуры

        self.conv1_1 = nn.Conv2d(input_shape[1], conv_shapes[0], kernel_size=7, padding=3)
        self.conv1_2 = nn.Conv2d(conv_shapes[0], conv_shapes[0], kernel_size=3, padding=1)

        self.conv2_1 = nn.Conv2d(conv_shapes[0], conv_shapes[1], kernel_size=3, padding=1)
        self.conv2_2 = nn.Conv2d(conv_shapes[1], conv_shapes[1], kernel_size=3, padding=1)
        self.conv2_3 = nn.Conv2d(conv_shapes[0], conv_shapes[1], kernel_size=1)

        self.conv3_1 = nn.Conv2d(conv_shapes[1], conv_shapes[2], kernel_size=3, padding=1)
        self.conv3_2 = nn.Conv2d(conv_shapes[2], conv_shapes[2], kernel_size=3, padding=1)
        self.conv3_3 = nn.Conv2d(conv_shapes[1], conv_shapes[2], kernel_size=1)

        self.conv4_1 = nn.Conv2d(conv_shapes[2], conv_shapes[3], kernel_size=3, padding=1)
        self.conv4_2 = nn.Conv2d(conv_shapes[3], conv_shapes[3], kernel_size=3, padding=1)
        self.conv4_3 = nn.Conv2d(conv_shapes[2], conv_shapes[3], kernel_size=1)

        self.conv5_1 = nn.Conv2d(conv_shapes[3], conv_shapes[4], kernel_size=3, padding=1)
        self.conv5_2 = nn.Conv2d(conv_shapes[4], conv_shapes[4], kernel_size=3, padding=1)
        self.conv5_3 = nn.Conv2d(conv_shapes[3], conv_shapes[4], kernel_size=1)

        self.conv6_1 = nn.Conv2d(input_shape[4], conv_shapes[5], kernel_size=3, padding=1)
        self.conv6_2 = nn.Conv2d(conv_shapes[5], conv_shapes[5], kernel_size=3, padding=1)

        # input_shape = (N, col, w, h)
        # каждый  pooling (h, w) -> (h // 2 + 1, w // 2 + 1)

        (fc_w, fc_h) = (input_shape[2], input_shape[3])
        for i in range(6):
            (fc_w, fc_h) = [x // 2 + 1 for x in (fc_w, fc_h)]

        self.fc = nn.Linear(conv_shapes[-1] * fc_w * fc_h, fc_shapes[0])
        self.W_in = FullyCon3D(fc_shapes[0], conv3_shape, out_shape)
        self.W_fg = FullyCon3D(fc_shapes[0], conv3_shape, out_shape)
        self.W_out = FullyCon3D(fc_shapes[0], conv3_shape, out_shape)

    def forward(self, x, hidden):

        if self.head:
            header = self.head(x)
            header = header.reshape(header.shape[0], -1)
            fc_header = self.relu(self.fc(header))

        else:
            header = self.pool(self.relu(self.conv1_2(self.relu(self.conv1_1(x)))))
            
            help = self.pool(self.relu(self.conv2_2(self.relu(self.conv2_1(header)))))
            header = self.pool(help + self.relu(self.conv2_3(x)))

            help = self.pool(self.relu(self.conv3_2(self.relu(self.conv3_1(header)))))
            header = self.pool(help + self.relu(self.conv3_3(x)))

            help = self.pool(self.relu(self.conv4_2(self.relu(self.conv4_1(header)))))
            header = self.pool(help + self.relu(self.conv4_3(x)))

            help = self.pool(self.relu(self.conv5_2(self.relu(self.conv5_1(header)))))
            header = self.pool(help + self.relu(self.conv5_3(x)))

            header = self.pool(self.relu(self.conv5_2(self.relu(self.conv5_1(header)))))

            header = header.reshape(header.shape[0], -1)
            dc_header = self.relu(self.fc(header))

        i = F.sigmoid(self.W_in(fc_header, hidden))
        f = F.sigmoid(self.W_fg(fc_header, hidden))

        h_new = F.tanh(self.W_out(fc_header, f * hidden))

        return i * hidden + (1 - i) * h_new, i

class GRU_Decoder(nn.Module):
    '''
    декодер это декодер
    '''
    def __init__(self, deconv_shapes):

        if (len(deconv_shapes) != 6):
            print("ERROR")
            return None 

        super().__init__()

        self.pool = None
        
        self.deconv1_1 = nn.Conv3d(deconv_shapes[0], deconv_shapes[1], kernel_size=7, padding=3)
        self.deconv1_2 = nn.Conv3d(deconv_shapes[1], deconv_shapes[1], kernel_size=3, padding=1)

        self.deconv2_1 = nn.Conv3d(deconv_shapes[1], deconv_shapes[2], kernel_size=3, padding=1)
        self.deconv2_2 = nn.Conv3d(deconv_shapes[2], deconv_shapes[2], kernel_size=3, padding=1)

        self.deconv3_1 = nn.Conv3d(deconv_shapes[2], deconv_shapes[3], kernel_size=3, padding=1)
        self.deconv3_2 = nn.Conv3d(deconv_shapes[3], deconv_shapes[3], kernel_size=3, padding=1)
        self.deconv3_3 = nn.Conv3d(deconv_shapes[2], deconv_shapes[3], kernel_size=1)

        self.deconv4_1 = nn.Conv3d(deconv_shapes[3], deconv_shapes[4], kernel_size=3, padding=1)
        self.deconv4_2 = nn.Conv3d(deconv_shapes[4], deconv_shapes[4], kernel_size=3, padding=1)
        self.deconv4_3 = nn.Conv3d(deconv_shapes[3], deconv_shapes[4], kernel_size=1)

        self.deconv6_1 = nn.Conv3d(deconv_shapes[4], deconv_shapes[5], kernel_size=3, padding=1)
        
        self.relu = nn.LeakyReLU(negative_slope=0.01)
        self.pool = Pool3D(kernel_size=2, padding=0)

    def forawrd(self, x):

        pool = self.pool(x)
        header = pool + self.relu(self.deconv1_2(self.relu(self.deconv1_1(pool))))

        pool = self.pool(header)
        header = pool + self.relu(self.deconv2_2(self.relu(self.deconv2_1(pool))))

        pool = self.pool(header)
        header = self.deconv3_3(pool) + self.relu(self.deconv3_2(self.relu(self.deconv3_1(pool))))

        pool = self.pool(header)
        header = self.deconv4_3(pool) + self.relu(self.deconv4_2(self.relu(self.deconv4_1(pool))))

        pool = self.pool(header)

        return self.deconv6_1(pool)

class GRU(nn.Module):

    def __init__(self, input_shape, nput_shape, conv_shapes, fc_shapes, conv3_shape, out_shape, deconv_shapes,
        res=True):

        super().__init__() 
        self.encoder = GRU_Encoder(input_shape, nput_shape, conv_shapes, fc_shapes, conv3_shape, out_shape)
        self.decoder = GRU_Decoder(deconv_shapes)
        self.loss_fn = SoftMax3D(False)
    
    def forward(self, x, y=None, verbose=False):

        hidden_history = []
        for i in range(x.shape[0]):
            hidden, hidden_ = self.encoder(x[i], hidden)
            hidden_history.append(hidden_)
        out = self.decoder(hidden)
        return self.loss_fn(hidden, y, verbose)

