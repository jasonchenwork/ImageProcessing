
# Saliency Map

A saliency map is an image that highlights region on which people's eyes focus first.
I implemented a spectral residual approach.
This approach is as:
$$ A= Amplitude(F(I))$$ 
$$ P= Phase(F(I))$$ 
$$L = log(A)$$
$$ Residual = L - L *( meanfilter) $$
$$ SaliencyMap =g * (F^{-1}(exp(Residual+i*P)))^2 $$
$$ SaliencyMap =g * (F^{-1}(exp(Residual)cos(P),exp(Residual)sin(P))))^2 $$