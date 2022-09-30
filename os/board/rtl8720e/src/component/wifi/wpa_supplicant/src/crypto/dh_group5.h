#ifndef DH_GROUP5_H
#define DH_GROUP5_H

void *dh5_init(struct wpabuf **priv, struct wpabuf **publ);
void *dh5_init_fixed(const struct wpabuf *priv, const struct wpabuf *publ);
struct wpabuf *dh5_derive_shared(void *ctx, const struct wpabuf *peer_public,
								 const struct wpabuf *own_private);
void dh5_free(void *ctx);

#endif /* DH_GROUP5_H */
