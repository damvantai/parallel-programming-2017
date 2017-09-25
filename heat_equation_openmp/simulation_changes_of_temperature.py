
# coding: utf-8

# In[29]:


import matplotlib.pyplot as plt


# In[30]:


import numpy as np


# In[31]:


import pandas as pd


# In[32]:


data = []


# In[33]:


with open("data.txt", "r") as file:
    for line in file.readlines():
        data.append(line.split(" "))
file.close()


# In[34]:


data = np.asarray(data)


# In[35]:


data.shape


# In[36]:


type(data)


# In[37]:


temperture = np.empty([100, 20])


# In[38]:


time = np.empty([100,1])


# In[39]:


for i in range(100):
    time[i] = data[i][0]


# In[40]:


temperture = data[0:100, 1:21]


# In[41]:


temperture.shape


# In[42]:


time.shape


# In[43]:


x = np.arange(0, 2, 0.1)


# In[58]:


x


# In[60]:


temperture


# In[72]:


from matplotlib.animation import FuncAnimation

fig = plt.figure()

def update(i):
    plt.plot(x, temperture[i], 'r')

anim = FuncAnimation(fig, update, interval=200)
anim.save("heat_equation.mp4")
plt.show()

