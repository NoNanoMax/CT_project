import torch
import torch.functional as F
import torch.nn as nn

class FullyCon3D(nn.Module):
    '''
    3-D свертка
    '''
    def __init__(self, input_shape, kernel_size, output_shape, 
            config={'dtype': torch.float32, 'device': 'cpu'}):
        super().__init__()
        self.input_shape = input_shape
        self.kernel_size = kernel_size
        self.output_shape = output_shape

        fc_out_shape = 1
        for shape in output_shape[1:]:
            fc_out_shape *= shape

        self.fc = nn.Linear(input_shape, fc_out_shape, bias=False, device=config["device"])
        self.fc_bias = nn.Parameter(torch.ones(1, output_shape[1], 1, 1, 1, **config))

        self.conv = nn.Conv3d(*kernel_size, padding=(kernel_size[2] - 1)//2, bias=False)
    
    def forward(self, x, hidden):
        '''
        Input: x - входное изображение
               hidden - скрытый слой прошлого шага, 3-мерный (N, col, w, h, d)
        Return: 3D- свертка hidden со смещением на x
        '''
        # x = (N, 3, h, w, c)
        to_reshape = self.output_shape
        to_reshape[0] = -1
        return self.conv(hidden) + self.fc(x).reshape(*to_reshape) + self.fc_bias

class Pool3D(nn.Module):
    '''
     Pooling
    '''
    def __init__(self, kernel_size, padding, config={'dtype': torch.float32, 'device': 'cpu'}):
        super().__init__()
        self.kernel_size = kernel_size
        self.padding = padding
        self.config = config
    
    def forward(self, x):

        # x = (N, col, h, w, c)
        # пока наивная реализация, просто заполню кубики kernel_size * kernel_size * kernel_size

        unpooled = torch.zeros(x.shape[0], x.shape[1], x.shape[2] * self.kernel_size,
            x.shape[3] * self.kernel_size, x.shape[4] * self.kernel_size, **self.config) 
        
        unpooled[:, :,self.padding : self.padding + x.shape[2] + 1 : self.kernel_size, 
                self.padding : self.padding + x.shape[3] + 1 : self.kernel_size,
                self.padding : self.padding + x.shape[4] + 1 : self.kernel_size] = x

class SoftMax3D(nn.Module):
    '''
    Вычисляет обычный софтмакс, но для трехмерного случая,
    вместе с этим считает кросс-энтропию(должно выйти быстрее, хз и в torch реализация смущает)
    '''
    def __init__(self, to_sum=False):
        super().__init__()
        self.to_sum = to_sum
        self.loss = 0
    
    def forward(self, x, y=None, verbose=False):
        
        if y is None:
            print("Error")
            return None
        
        # loss = -log(exp(x_i) / sum(exp(x_j))) = log(sum(exp(x_j - x_i)))

        exp_loss = x - torch.max(x, dim=1, keepdim=True)[0] # выделил x_j - x_i
        sum_exp_loss = torch.sum(torch.exp(exp_loss), dim = 1, keepdim = True)
        self.loss = torch.mean(-torch.sum(y * exp_loss, dim = 1, keepdim = True) +
                              torch.log(sum_exp_loss))

        preds = exp_loss / sum_exp_loss
        return self.loss, preds